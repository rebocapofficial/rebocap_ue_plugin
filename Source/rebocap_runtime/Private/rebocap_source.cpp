#include "rebocap_source.h"
#include "rebocap_ws_sdk_cpp.h"
#include "rebocap_skeleton_data.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "ILiveLinkClient.h" 
#include "Async/Async.h"

// 静态变量初始化
TSharedPtr<FRebocapSource> FRebocapSource::instance_ = nullptr;
bool FRebocapSource::bAutoSkeleton = true; 

FRebocapSource::FRebocapSource(uint16_t port) 
    : port_(port)
{
    client_ = nullptr;
    
    // 初始化 SDK 对象
    rebocap_sdk_ = MakeShared<rebocap::RebocapWsSdk>(CoordinateSpaceType::UECoordinate, true);
    
    // 绑定回调
    rebocap_sdk_->SetPoseMsgCallback([this](const QuatMsg* msg, rebocap::RebocapWsSdk* h) { 
        this->pose_msg_callback(msg); 
    });
    
    // 设置异常断开回调
    rebocap_sdk_->SetExceptionCloseCallback([this](rebocap::RebocapWsSdk* h) { 
        status_ = false; 
    });

    // ================== 【修复开始】 ==================
    // 关键修复：检测是否在运行 Commandlet (例如打包 Cook 过程)
    // 如果是，则绝对不要启动连接线程！防止 SECURE CRT 崩溃。
    if (IsRunningCommandlet())
    {
        UE_LOG(LogTemp, Warning, TEXT("Rebocap: Skipped connection thread during Commandlet/Cooking mode."));
        // 直接返回，不创建线程
        return;
    }
    // ================== 【修复结束】 ==================

    FString ThreadName(FString::Printf(TEXT("FRebocapSource_%d_%lld"), port, FDateTime::UtcNow().ToUnixTimestamp()));
    thread_ = FRunnableThread::Create(this, *ThreadName, 512 * 1024, TPri_Normal);

    UE_LOG(LogTemp, Display, TEXT("New Rebocap Source created on port: %d"), port);
}

FRebocapSource::FRebocapSource(const FString& IPAddress, uint16_t port)
    : FRebocapSource(port) 
{
}

FRebocapSource::~FRebocapSource() {
    UE_LOG(LogTemp, Display, TEXT("Destroying Rebocap Source..."));

    // 1. 停止标志：阻止新数据处理
    bIsShuttingDown = true;
    running_ = false; 

    // 2. 线程同步：必须等待 Run() 函数彻底退出
    if (thread_ != nullptr) {
        thread_->WaitForCompletion(); 
        delete thread_;
        thread_ = nullptr;
    }

    // 3. 清理 LiveLink
    clear_all_subjects();

    // 4. 销毁 SDK：此时线程已安全停止，可以释放内存
    if (rebocap_sdk_.IsValid()) {
        rebocap_sdk_->UnregisterCallbacks();
        rebocap_sdk_->Close();
        rebocap_sdk_.Reset();
    }
}

void FRebocapSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) {
    client_ = InClient;
    source_guid_ = InSourceGuid;
}

void FRebocapSource::InitializeSettings(ULiveLinkSourceSettings* Settings) {
    saved_source_settings_ = Cast<URebocapSourceSettings>(Settings);
}

bool FRebocapSource::IsSourceStillValid() const { 
    return client_ != nullptr; 
}

bool FRebocapSource::RequestSourceShutdown() {
    clear_all_subjects();
    return true;
}

FText FRebocapSource::GetSourceType() const { return FText::FromString("Rebocap"); }
FText FRebocapSource::GetSourceMachineName() const { return FText::FromString(FString::Printf(TEXT("UDP:%d"), port_)); }
FText FRebocapSource::GetSourceStatus() const { return status_ ? FText::FromString("Active") : FText::FromString("Connecting"); }

TSharedPtr<FRebocapSource> FRebocapSource::GetInstance() { return instance_; }
void FRebocapSource::SetInstance(TSharedPtr<FRebocapSource> instance) { instance_ = instance; }

void FRebocapSource::clear_all_subjects() {
    if (client_ == nullptr) return;
    
    for (const auto& subject_name : subject_names_) {
        client_->RemoveSubject_AnyThread(FLiveLinkSubjectKey(source_guid_, subject_name));
    }
    subject_names_.Empty();
}

bool FRebocapSource::open() {
    if (status_) return true;
    
    if (!rebocap_sdk_.IsValid()) return false;

    int ret = rebocap_sdk_->Open(port_, "reborn_app", 114514);
    status_ = (ret == 0);
    
    if (status_) {
        UE_LOG(LogTemp, Display, TEXT("Rebocap SDK Connected on port: %d!"), port_);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Rebocap SDK Open failed on port %d (ret: %d)"), port_, ret);
    }
    return status_;
}

bool FRebocapSource::Init() { return true; }

uint32 FRebocapSource::Run() {
    uint16_t tick = 0;
    while (running_) {
        if (tick++ % 10 == 0) {
            open();
        }
        FPlatformProcess::Sleep(0.033f);
    }
    return 0;
}

void FRebocapSource::Stop() {
    running_ = false;
    if (thread_) {
        thread_->WaitForCompletion();
    }
}

void FRebocapSource::ManualStop() {
    running_ = false;
}

bool FRebocapSource::ManualStart(uint16_t port) {
    // 【修复】防止在 Cook 模式下手动启动
    if (IsRunningCommandlet()) return false;

    port_ = port;
    if (!running_ && !status_) {
        running_ = true;
        FString ThreadName(FString::Printf(TEXT("FRebocapSource_%d_Restart"), port));
        thread_ = FRunnableThread::Create(this, *ThreadName, 512 * 1024, TPri_Normal);
    }
    return running_;
}

int FRebocapSource::CalculateAndRegisterToRebocap(
    TArray<FVector3f>& left_vertex, TArray<FVector3f>& left_normals,
    TArray<FVector3f>& right_vertex, TArray<FVector3f>& right_normals,
    TArray<FVector3f>& skeletons, const std::string& coordinate,
    TArray<FVector3f>& foot_vertices) 
{
    if (rebocap_sdk_.IsValid()) {
        return rebocap_sdk_->CalculateAndRegisterToRebocap(
            left_vertex, left_normals, right_vertex, right_normals, skeletons, coordinate, foot_vertices);
    }
    return -1;
}

void FRebocapSource::pose_msg_callback(const QuatMsg* msg) {
    // 双重检查，防止析构时执行
    if (bIsShuttingDown || !client_ || !running_ || IsEngineExitRequested()) {
        return;
    }

    if (!msg) return;

    FName subject = "rebocap";
    
    // 1. 推送静态数据
    {
        FLiveLinkStaticDataStruct static_data_struct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
        FLiveLinkSkeletonStaticData& static_data = *static_data_struct.Cast<FLiveLinkSkeletonStaticData>();

        static TArray<FName> bone_names = {
            rebocap_bones::pelvis,  rebocap_bones::l_hip,      rebocap_bones::r_hip,      rebocap_bones::spine1,   rebocap_bones::l_knee,
            rebocap_bones::r_knee,  rebocap_bones::spine2,     rebocap_bones::l_ankle,    rebocap_bones::r_ankle,  rebocap_bones::spine3,
            rebocap_bones::l_foot,  rebocap_bones::r_foot,     rebocap_bones::neck,       rebocap_bones::l_collar, rebocap_bones::r_collar,
            rebocap_bones::head,    rebocap_bones::l_shoulder, rebocap_bones::r_shoulder, rebocap_bones::l_elbow,  rebocap_bones::r_elbow,
            rebocap_bones::l_wrist, rebocap_bones::r_wrist,    rebocap_bones::l_hand,     rebocap_bones::r_hand,
        };

        static TArray<int32> bone_parents = {
            -1, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 12, 13, 14, 16, 17, 18, 19, 20, 21,
        };

        static_data.SetBoneNames(bone_names);
        static_data.SetBoneParents(bone_parents);

        if (!subject_names_.Contains(subject)) {
            client_->PushSubjectStaticData_AnyThread({source_guid_, subject}, ULiveLinkAnimationRole::StaticClass(), MoveTemp(static_data_struct));
            subject_names_.Add(subject);
        }
    }

    // 2. 推送帧数据
    FLiveLinkFrameDataStruct frame = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
    FLiveLinkBaseFrameData* base_data = frame.GetBaseData();
    base_data->WorldTime = FLiveLinkWorldTime(FPlatformTime::Seconds());
    
    FLiveLinkAnimationFrameData& frame_data = *frame.Cast<FLiveLinkAnimationFrameData>();
    frame_data.Transforms.Reserve(24);

    for (int i = 0; i < 96; i += 4) {
        FTransform transform;
        if (i == 0) {
             transform.SetTranslation(FVector(msg->trans[0], msg->trans[1], msg->trans[2]));
        } else {
             transform.SetTranslation(FVector::ZeroVector);
        }
        
        // 【关键修复：防止频闪】
        // 获取原始四元数
        FQuat RawRot(msg->quat[i], msg->quat[i + 1], msg->quat[i + 2], msg->quat[i + 3]);
        
        // 检查是否为“脏数据”（全0或未归一化）
        if (RawRot.IsIdentity() || RawRot.SizeSquared() < 1.e-4f) {
            // 如果数据坏了，保持默认旋转（防止模型坍缩）
            transform.SetRotation(FQuat::Identity);
        } else {
            // 如果数据正常，强制归一化（防止模型抽搐）
            RawRot.Normalize();
            transform.SetRotation(RawRot);
        }
        
        transform.SetScale3D(FVector::OneVector);
        
        frame_data.Transforms.Add(MoveTemp(transform));
    }

    client_->PushSubjectFrameData_AnyThread({source_guid_, subject}, MoveTemp(frame));
}