#pragma once

#include <string>
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "ILiveLinkClient.h"
#include "ILiveLinkSource.h"
#include "rebocap_source_settings.h"
#include "rebocap_ws_sdk_cpp.h"

class REBOCAP_RUNTIME_API FRebocapSource final : public ILiveLinkSource, public FRunnable
{
public:
    FRebocapSource(uint16_t port);
    FRebocapSource(const FString& IPAddress, uint16_t port);
    virtual ~FRebocapSource() override;

    // --- LiveLink 接口 ---
    virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;
    virtual void InitializeSettings(ULiveLinkSourceSettings* Settings) override;
    virtual bool IsSourceStillValid() const override;
    virtual bool RequestSourceShutdown() override;
    virtual FText GetSourceType() const override;
    virtual FText GetSourceMachineName() const override;
    virtual FText GetSourceStatus() const override;

    // --- 功能接口 ---
    int CalculateAndRegisterToRebocap(
        TArray<FVector3f>& left_vertex,
        TArray<FVector3f>& left_normals,
        TArray<FVector3f>& right_vertex,
        TArray<FVector3f>& right_normals,
        TArray<FVector3f>& skeletons,
        const std::string& coordinate,
        TArray<FVector3f>& foot_vertices);

    bool IsPortOpen() const { return status_; }
    void ManualStop();
    bool ManualStart(uint16_t port);

    // --- 单例管理 ---
    static TSharedPtr<FRebocapSource> GetInstance();
    static void SetInstance(TSharedPtr<FRebocapSource> instance);

    // --- 全局静态开关 ---
    static bool bAutoSkeleton;          
    static bool bUserOverrideDisconnect; // 保留定义以防链接错误

private:
    // --- FRunnable 线程接口 ---
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;

    // 内部函数
    void clear_all_subjects();
    bool open();
    void pose_msg_callback(const QuatMsg* msg);

private:
    ILiveLinkClient* client_ = nullptr;
    FGuid source_guid_;
    URebocapSourceSettings* saved_source_settings_ = nullptr;
    TArray<FName> subject_names_;
    
    uint16_t port_;
    bool status_{false}; // 连接状态

    // 线程相关
    FRunnableThread* thread_{nullptr};
    bool running_{true};     
    bool bIsShuttingDown{false}; 

    // SDK指针
    TSharedPtr<rebocap::RebocapWsSdk> rebocap_sdk_;

    // 单例静态指针
    static TSharedPtr<FRebocapSource> instance_;
};