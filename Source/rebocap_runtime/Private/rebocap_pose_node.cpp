#include "rebocap_pose_node.h"

#include "Roles/LiveLinkAnimationRole.h"
#include "rebocap_skeleton_data.h"
#include "rebocap_source.h" 
#include "rebocap_ws_sdk_cpp.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Features/IModularFeatures.h"
#include "LiveLinkClient.h"
#include "ILiveLinkClient.h"
#include "LiveLinkRemapAsset.h"
#include "UObject/Package.h" 

FRebocapPoseNode::FRebocapPoseNode()
    : LiveLinkSubjectName("rebocap")
{
}

void FRebocapPoseNode::UpdateConnectionState()
{
    auto CurrentSource = FRebocapSource::GetInstance();
    IModularFeatures& ModularFeatures = IModularFeatures::Get();
    if (!ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName)) return;
    ILiveLinkClient* LiveLinkClient = &ModularFeatures.GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
    if (!LiveLinkClient) return;

    if (bAutoConnect)
    {
        if (!CurrentSource.IsValid())
        {
            uint16_t Port = 7690;
            TSharedPtr<FRebocapSource> NewSource = MakeShared<FRebocapSource>(Port);
            FRebocapSource::SetInstance(NewSource);
            LiveLinkClient->AddSource(NewSource);
        }
        else if (!CurrentSource->IsPortOpen())
        {
            CurrentSource->ManualStart(7690);
        }
    }
    else
    {
        if (CurrentSource.IsValid())
        {
            CurrentSource->ManualStop();
            LiveLinkClient->RemoveSource(CurrentSource);
            FRebocapSource::SetInstance(nullptr);
        }
    }
}

void FRebocapPoseNode::OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance) {
  Super::OnInitializeAnimInstance(InProxy, InAnimInstance);
  FRebocapSource::bAutoSkeleton = bAutoSkeleton;
}

void FRebocapPoseNode::GatherDebugData(FNodeDebugData& DebugData) {
  const FString DebugLine = DebugData.GetNodeName(this);
  DebugData.AddDebugItem(DebugLine);
  ComponentPose.GatherDebugData(DebugData);
}

static inline void original_transform(FBoneReference bone, FCSPose<FCompactPose>& mesh_bases, std::optional<FTransform>& trans) {
  if (trans) return;
  const FBoneContainer& bone_container = mesh_bases.GetPose().GetBoneContainer();
  FCompactPoseBoneIndex compact_pose_bone_to_modify = bone.GetCompactPoseIndex(bone_container);
  if (compact_pose_bone_to_modify == -1) return;
  FTransform new_bone_tm = mesh_bases.GetComponentSpaceTransform(compact_pose_bone_to_modify);
  trans = new_bone_tm;
}

static inline FQuat get_rotation3(const FName& bone_name, FLiveLinkSubjectFrameData& in_subject_frame_data) {
  FLiveLinkSkeletonStaticData* skeleto_data = in_subject_frame_data.StaticData.Cast<FLiveLinkSkeletonStaticData>();
  FLiveLinkAnimationFrameData* frame_data = in_subject_frame_data.FrameData.Cast<FLiveLinkAnimationFrameData>();
  int32 index = -1;
  if (auto rebocap_bone = skeleto_data->BoneNames.Find(bone_name, index)) {
    return frame_data->Transforms[index].GetRotation();
  }
  return FQuat::Identity;
}

static inline void get_rotation3_location3(const FName& bone_name, FLiveLinkSubjectFrameData& in_subject_frame_data, FQuat& rotation,
                                           FVector& location) {
  FLiveLinkSkeletonStaticData* SkeletonData = in_subject_frame_data.StaticData.Cast<FLiveLinkSkeletonStaticData>();
  FLiveLinkAnimationFrameData* frame_data = in_subject_frame_data.FrameData.Cast<FLiveLinkAnimationFrameData>();
  int32 index = -1;
  if (auto rebocap_bone = SkeletonData->BoneNames.Find(bone_name, index)) {
    rotation = frame_data->Transforms[index].GetRotation();
    location = frame_data->Transforms[index].GetLocation();
    return;
  }
  rotation = FQuat::Identity;
  location = FVector::ZeroVector;
}

static void apply_rebocap_transform(FBoneReference bone, FQuat quat, FVector position, FCSPose<FCompactPose>& mesh_bases) {
  if (bone.BoneIndex == -1) return;
  const auto& bone_container = mesh_bases.GetPose().GetBoneContainer();
  FCompactPoseBoneIndex compact_pose_bone_to_modify = bone.GetCompactPoseIndex(bone_container);
  FTransform new_bone_tm = mesh_bases.GetComponentSpaceTransform(compact_pose_bone_to_modify);
  new_bone_tm.SetRotation(quat);
  new_bone_tm.SetTranslation(position);
  mesh_bases.SetComponentSpaceTransform(compact_pose_bone_to_modify, new_bone_tm);
}

static void apply_rebocap_transform(FBoneReference bone, FQuat quat, FCSPose<FCompactPose>& mesh_bases) {
  if (bone.BoneIndex == -1) return;
  const auto& bone_container = mesh_bases.GetPose().GetBoneContainer();
  FCompactPoseBoneIndex compact_pose_bone_to_modify = bone.GetCompactPoseIndex(bone_container);
  FTransform new_bone_tm = mesh_bases.GetComponentSpaceTransform(compact_pose_bone_to_modify);
  new_bone_tm.SetRotation(quat);
  mesh_bases.SetComponentSpaceTransform(compact_pose_bone_to_modify, new_bone_tm);
}

static void Init_Tpose_Bone(FRebocapBodyMap& bone_map, FCSPose<FCompactPose>& mesh_bases, TPose& t_pose) {
  original_transform(bone_map.Pelvis, mesh_bases, t_pose.pelvis);
  original_transform(bone_map.L_Hip, mesh_bases, t_pose.l_hip);
  original_transform(bone_map.R_Hip, mesh_bases, t_pose.r_hip);
  original_transform(bone_map.Spine1, mesh_bases, t_pose.spine1);
  original_transform(bone_map.L_Knee, mesh_bases, t_pose.l_knee);
  original_transform(bone_map.R_Knee, mesh_bases, t_pose.r_knee);
  original_transform(bone_map.Spine2, mesh_bases, t_pose.spine2);
  original_transform(bone_map.L_Ankle, mesh_bases, t_pose.l_ankle);
  original_transform(bone_map.R_Ankle, mesh_bases, t_pose.r_ankle);
  original_transform(bone_map.Spine3, mesh_bases, t_pose.spine3);
  original_transform(bone_map.L_Foot, mesh_bases, t_pose.l_foot);
  original_transform(bone_map.R_Foot, mesh_bases, t_pose.r_foot);
  original_transform(bone_map.Neck, mesh_bases, t_pose.neck);
  original_transform(bone_map.L_Collar, mesh_bases, t_pose.l_collar);
  original_transform(bone_map.R_Collar, mesh_bases, t_pose.r_collar);
  original_transform(bone_map.Head, mesh_bases, t_pose.head);
  original_transform(bone_map.L_Shoulder, mesh_bases, t_pose.l_shoulder);
  original_transform(bone_map.R_Shoulder, mesh_bases, t_pose.r_shoulder);
  original_transform(bone_map.L_Elbow, mesh_bases, t_pose.l_elbow);
  original_transform(bone_map.R_Elbow, mesh_bases, t_pose.r_elbow);
  original_transform(bone_map.L_Wrist, mesh_bases, t_pose.l_wrist);
  original_transform(bone_map.R_Wrist, mesh_bases, t_pose.r_wrist);
  original_transform(bone_map.L_Hand, mesh_bases, t_pose.l_hand);
  original_transform(bone_map.R_Hand, mesh_bases, t_pose.r_hand);
}

#if WITH_EDITOR
void GetVerticesAndNormals(USkeletalMeshComponent* MeshComp, FBoneReference bone0, FBoneReference bone1,
                           TArray<FVector3f>& OutVertices, TArray<FVector3f>& OutNormals) {
  if (!MeshComp || !MeshComp->GetSkinnedAsset()) return;
  FSkeletalMeshRenderData* RenderData = MeshComp->GetSkinnedAsset()->GetResourceForRendering();
  FSkeletalMeshModel* RenderModel = MeshComp->GetSkinnedAsset()->GetImportedModel();
  if (!RenderData || RenderData->LODRenderData.Num() == 0 || RenderModel->LODModels.Num() == 0) return;
  const int32 BoneIndex0 = MeshComp->GetBoneIndex(bone0.BoneName);
  const int32 BoneIndex1 = MeshComp->GetBoneIndex(bone1.BoneName);
  if (BoneIndex0 == INDEX_NONE && BoneIndex1 == INDEX_NONE) return;
  auto& LODModel = RenderModel->LODModels[0];
  for (int32 SectionIndex = 0; SectionIndex < LODModel.Sections.Num(); SectionIndex++) {
    if (!LODModel.Sections[SectionIndex].BoneMap.Contains(BoneIndex0) && !LODModel.Sections[SectionIndex].BoneMap.Contains(BoneIndex1))
      continue;
    const FSkelMeshSection& Section = LODModel.Sections[SectionIndex];
    for (auto& Vertex : Section.SoftVertices) {
      for (int32 j = 0; j < Section.MaxBoneInfluences; ++j) {
        if (Vertex.InfluenceWeights[j] > 0 &&
            (Section.BoneMap[Vertex.InfluenceBones[j]] == BoneIndex0 || Section.BoneMap[Vertex.InfluenceBones[j]] == BoneIndex1)) {
          OutVertices.Add(Vertex.Position);
          OutNormals.Add(Vertex.TangentZ);
          break;
        }
      }
    }
  }
}
#endif

inline void AddBoneLocation(const std::optional<FTransform>& In_Trans, TArray<FVector3f>& SkeletonPosition, const int Parent_Node) {
  FVector3f Translation3F;
  if (In_Trans) {
    const FVector In = In_Trans.value().GetTranslation();
    Translation3F.X = In.X;
    Translation3F.Y = In.Y;
    Translation3F.Z = In.Z;
  } else {
    Translation3F = SkeletonPosition[Parent_Node] + FVector3f(1e-9f, 1e-9f, 1e-9f);
  }
  SkeletonPosition.Add(Translation3F);
}

inline void MeterToCentimeter(TArray<FVector3f> &VecArray) {
  for (auto& Vec : VecArray) Vec /= 100.0f;
}

void FRebocapPoseNode::Init_Foot_Vertices_And_SkeletalData(USkeletalMeshComponent* MeshComp) {
  const auto Source = FRebocapSource::GetInstance();
  if (!Source.IsValid() || !Source->IsPortOpen()) {
      init_vertices_ = false; 
      return;
  }
  if (init_vertices_) return;
  if (!FRebocapSource::bAutoSkeleton) return; 

  if (t_pose_.pelvis) {
    if (LeftVertices_.Num() == 0 && RightVertices_.Num() == 0) {
#if WITH_EDITOR
      GetVerticesAndNormals(MeshComp, bone_map_.L_Ankle, bone_map_.L_Foot, LeftVertices_, LeftNormals_);
      GetVerticesAndNormals(MeshComp, bone_map_.R_Ankle, bone_map_.R_Foot, RightVertices_, RightNormals_);
      MeterToCentimeter(LeftVertices_);
      MeterToCentimeter(RightVertices_);
#endif
    }
    if (SkeletonPosition_.Num() == 0) {
       AddBoneLocation(t_pose_.pelvis, SkeletonPosition_, REBOCAP_Pelvis_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_hip, SkeletonPosition_, REBOCAP_L_Hip_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_hip, SkeletonPosition_, REBOCAP_R_Hip_PARENTS_BONE);
       AddBoneLocation(t_pose_.spine1, SkeletonPosition_, REBOCAP_Spine1_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_knee, SkeletonPosition_, REBOCAP_L_Knee_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_knee, SkeletonPosition_, REBOCAP_R_Knee_PARENTS_BONE);
       AddBoneLocation(t_pose_.spine2, SkeletonPosition_, REBOCAP_Spine2_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_ankle, SkeletonPosition_, REBOCAP_L_Ankle_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_ankle, SkeletonPosition_, REBOCAP_R_Ankle_PARENTS_BONE);
       AddBoneLocation(t_pose_.spine3, SkeletonPosition_, REBOCAP_Spine3_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_foot, SkeletonPosition_, REBOCAP_L_Foot_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_foot, SkeletonPosition_, REBOCAP_R_Foot_PARENTS_BONE);
       AddBoneLocation(t_pose_.neck, SkeletonPosition_, REBOCAP_Neck_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_collar, SkeletonPosition_, REBOCAP_L_Collar_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_collar, SkeletonPosition_, REBOCAP_R_Collar_PARENTS_BONE);
       AddBoneLocation(t_pose_.head, SkeletonPosition_, REBOCAP_Head_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_shoulder, SkeletonPosition_, REBOCAP_L_Shoulder_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_shoulder, SkeletonPosition_, REBOCAP_R_Shoulder_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_elbow, SkeletonPosition_, REBOCAP_L_Elbow_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_elbow, SkeletonPosition_, REBOCAP_R_Elbow_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_wrist, SkeletonPosition_, REBOCAP_L_Wrist_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_wrist, SkeletonPosition_, REBOCAP_R_Wrist_PARENTS_BONE);
       AddBoneLocation(t_pose_.l_hand, SkeletonPosition_, REBOCAP_L_Hand_PARENTS_BONE);
       AddBoneLocation(t_pose_.r_hand, SkeletonPosition_, REBOCAP_R_Hand_PARENTS_BONE);
       MeterToCentimeter(SkeletonPosition_);
    }

    if (Source.IsValid()) {
      TArray<FVector3f> FinalFootVertex;  
      const int Res = Source->CalculateAndRegisterToRebocap(LeftVertices_, LeftNormals_, RightVertices_,
                                                            RightNormals_, SkeletonPosition_, "xzy", FinalFootVertex);
      if (Res == 0) {
        init_vertices_ = true;
        // 日志只打印成功，消除卡顿
        UE_LOG(LogTemp, Display, TEXT("Regist rebocap SUCCESS!"));
      }
    }
  }
}

void FRebocapPoseNode::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) {
  USkeletalMeshComponent* skel_comp = Output.AnimInstanceProxy->GetSkelMeshComponent();
  FCSPose<FCompactPose>& mesh_bases = Output.Pose;
  check(OutBoneTransforms.Num() == 0);
  if (!live_link_client_) return;
  
  Init_Tpose_Bone(bone_map_, mesh_bases, t_pose_);
  Init_Foot_Vertices_And_SkeletalData(skel_comp);
  
  FLiveLinkSubjectFrameData subject_frame_data;
  FLiveLinkSubjectName live_link_subject_name = FName("rebocap");
  
  TSubclassOf<ULiveLinkRole> subject_role = live_link_client_->GetSubjectRole_AnyThread(live_link_subject_name);
  if (!subject_role) return;
  
  if (subject_role->IsChildOf(ULiveLinkAnimationRole::StaticClass())) {
    if (!live_link_client_->EvaluateFrame_AnyThread(live_link_subject_name, ULiveLinkAnimationRole::StaticClass(), subject_frame_data)) {
      return;
    }
  }
  FLiveLinkSkeletonStaticData* skeleton_data = subject_frame_data.StaticData.Cast<FLiveLinkSkeletonStaticData>();
  FLiveLinkAnimationFrameData* frame_data = subject_frame_data.FrameData.Cast<FLiveLinkAnimationFrameData>();
  check(skeleton_data);
  check(frame_data);

  FQuat pelvis_quat;
  FVector pelvis_position;
  get_rotation3_location3(rebocap_bones::pelvis, subject_frame_data, pelvis_quat, pelvis_position);
  auto l_hip_quat = get_rotation3(rebocap_bones::l_hip, subject_frame_data);
  auto r_hip_quat = get_rotation3(rebocap_bones::r_hip, subject_frame_data);
  auto spine1_quat = get_rotation3(rebocap_bones::spine1, subject_frame_data);
  auto l_knee_quat = get_rotation3(rebocap_bones::l_knee, subject_frame_data);
  auto r_knee_quat = get_rotation3(rebocap_bones::r_knee, subject_frame_data);
  auto spine2_quat = get_rotation3(rebocap_bones::spine2, subject_frame_data);
  auto l_ankle_quat = get_rotation3(rebocap_bones::l_ankle, subject_frame_data);
  auto r_ankle_quat = get_rotation3(rebocap_bones::r_ankle, subject_frame_data);
  auto spine3_quat = get_rotation3(rebocap_bones::spine3, subject_frame_data);
  auto l_foot_quat = get_rotation3(rebocap_bones::l_foot, subject_frame_data);
  auto r_foot_quat = get_rotation3(rebocap_bones::r_foot, subject_frame_data);
  auto neck_quat = get_rotation3(rebocap_bones::neck, subject_frame_data);
  auto l_collar_quat = get_rotation3(rebocap_bones::l_collar, subject_frame_data);
  auto r_collar_quat = get_rotation3(rebocap_bones::r_collar, subject_frame_data);
  auto head_quat = get_rotation3(rebocap_bones::head, subject_frame_data);
  auto l_shoulder_quat = get_rotation3(rebocap_bones::l_shoulder, subject_frame_data);
  auto r_shoulder_quat = get_rotation3(rebocap_bones::r_shoulder, subject_frame_data);
  auto l_elbow_quat = get_rotation3(rebocap_bones::l_elbow, subject_frame_data);
  auto r_elbow_quat = get_rotation3(rebocap_bones::r_elbow, subject_frame_data);
  auto l_wrist_quat = get_rotation3(rebocap_bones::l_wrist, subject_frame_data);
  auto r_wrist_quat = get_rotation3(rebocap_bones::r_wrist, subject_frame_data);
  auto l_hand_quat = get_rotation3(rebocap_bones::l_hand, subject_frame_data);
  auto r_hand_quat = get_rotation3(rebocap_bones::r_hand, subject_frame_data);

  const FBoneContainer& bone_container = mesh_bases.GetPose().GetBoneContainer();
  FCompactPoseBoneIndex compact_pose_bone_to_modify = bone_map_.Pelvis.GetCompactPoseIndex(bone_container);
  if (compact_pose_bone_to_modify == -1) return;
  
  {
    auto location = (pelvis_position * 100.0);  // M to CM
    apply_rebocap_transform(bone_map_.Pelvis, pelvis_quat * t_pose_.pelvis.value_or(FTransform()).GetRotation(), location,
                            mesh_bases);
  }
#define TRANSFORM(NAME1, NAME2)                                                                                                           \
  apply_rebocap_transform(bone_map_.NAME1, NAME2##_quat * t_pose_.NAME2.value_or(FTransform()).GetRotation(), mesh_bases)
  TRANSFORM(L_Hip, l_hip);
  TRANSFORM(R_Hip, r_hip);
  TRANSFORM(Spine1, spine1);
  TRANSFORM(L_Knee, l_knee);
  TRANSFORM(R_Knee, r_knee);
  TRANSFORM(Spine2, spine2);
  TRANSFORM(L_Ankle, l_ankle);
  TRANSFORM(R_Ankle, r_ankle);
  TRANSFORM(Spine3, spine3);
  TRANSFORM(L_Foot, l_foot);
  TRANSFORM(R_Foot, r_foot);
  TRANSFORM(Neck, neck);
  TRANSFORM(L_Collar, l_collar);
  TRANSFORM(R_Collar, r_collar);
  TRANSFORM(Head, head);
  TRANSFORM(L_Shoulder, l_shoulder);
  TRANSFORM(R_Shoulder, r_shoulder);
  TRANSFORM(L_Elbow, l_elbow);
  TRANSFORM(R_Elbow, r_elbow);
  TRANSFORM(L_Wrist, l_wrist);
  TRANSFORM(R_Wrist, r_wrist);
  TRANSFORM(L_Hand, l_hand);
  TRANSFORM(R_Hand, r_hand);
#undef TRANSFORM
}

void FRebocapPoseNode::PreUpdate(const UAnimInstance* InAnimInstance) {
  Super::PreUpdate(InAnimInstance);

  FRebocapSource::bAutoSkeleton = bAutoSkeleton;

  UpdateConnectionState();

  ILiveLinkClient* this_frame_client = nullptr;
  IModularFeatures& ModularFeatures = IModularFeatures::Get();
  if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName)) {
    this_frame_client = &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
  }
  live_link_client_ = this_frame_client;
  
  {
    UClass* retarget_asset_ptr = retarget_asset_.Get();
    if (!retarget_asset_ptr || retarget_asset_ptr->HasAnyClassFlags(CLASS_Abstract)) {
      retarget_asset_ptr = ULiveLinkRemapAsset::StaticClass();
      retarget_asset_ = retarget_asset_ptr;
    }
    if (!current_retarget_asset_ || retarget_asset_ptr != current_retarget_asset_->GetClass()) {
      current_retarget_asset_ = NewObject<ULiveLinkRemapAsset>(GetTransientPackage(), retarget_asset_ptr);
      current_retarget_asset_->Initialize();
    }
  }
}

void FRebocapPoseNode::InitializeBoneReferences(const FBoneContainer& RequiredBones) {
  if (current_retarget_asset_ != nullptr && current_retarget_asset_->IsValidLowLevel()) {
    InitBoneMap();
  }
  bone_map_.Pelvis.Initialize(RequiredBones);
  bone_map_.L_Hip.Initialize(RequiredBones);
  bone_map_.R_Hip.Initialize(RequiredBones);
  bone_map_.Spine1.Initialize(RequiredBones);
  bone_map_.L_Knee.Initialize(RequiredBones);
  bone_map_.R_Knee.Initialize(RequiredBones);
  bone_map_.Spine2.Initialize(RequiredBones);
  bone_map_.L_Ankle.Initialize(RequiredBones);
  bone_map_.R_Ankle.Initialize(RequiredBones);
  bone_map_.Spine3.Initialize(RequiredBones);
  bone_map_.L_Foot.Initialize(RequiredBones);
  bone_map_.R_Foot.Initialize(RequiredBones);
  bone_map_.Neck.Initialize(RequiredBones);
  bone_map_.L_Collar.Initialize(RequiredBones);
  bone_map_.R_Collar.Initialize(RequiredBones);
  bone_map_.Head.Initialize(RequiredBones);
  bone_map_.L_Shoulder.Initialize(RequiredBones);
  bone_map_.R_Shoulder.Initialize(RequiredBones);
  bone_map_.L_Elbow.Initialize(RequiredBones);
  bone_map_.R_Elbow.Initialize(RequiredBones);
  bone_map_.L_Wrist.Initialize(RequiredBones);
  bone_map_.R_Wrist.Initialize(RequiredBones);
  bone_map_.L_Hand.Initialize(RequiredBones);
  bone_map_.R_Hand.Initialize(RequiredBones);
}

void FRebocapPoseNode::InitBoneMap() {
  bone_map_.Pelvis = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::pelvis);
  bone_map_.L_Hip = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_hip);
  bone_map_.R_Hip = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_hip);
  bone_map_.Spine1 = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::spine1);
  bone_map_.L_Knee = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_knee);
  bone_map_.R_Knee = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_knee);
  bone_map_.Spine2 = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::spine2);
  bone_map_.L_Ankle = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_ankle);
  bone_map_.R_Ankle = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_ankle);
  bone_map_.Spine3 = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::spine3);
  bone_map_.L_Foot = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_foot);
  bone_map_.R_Foot = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_foot);
  bone_map_.Neck = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::neck);
  bone_map_.L_Collar = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_collar);
  bone_map_.R_Collar = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_collar);
  bone_map_.Head = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::head);
  bone_map_.L_Shoulder = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_shoulder);
  bone_map_.R_Shoulder = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_shoulder);
  bone_map_.L_Elbow = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_elbow);
  bone_map_.R_Elbow = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_elbow);
  bone_map_.L_Wrist = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_wrist);
  bone_map_.R_Wrist = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_wrist);
  bone_map_.L_Hand = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::l_hand);
  bone_map_.R_Hand = current_retarget_asset_->GetRemappedBoneName(rebocap_bones::r_hand);
}

void FRebocapPoseNode::UpdateComponentPose_AnyThread(const FAnimationUpdateContext& Context) {
  GetEvaluateGraphExposedInputs().Execute(Context);
  Super::UpdateComponentPose_AnyThread(Context);
}

void FRebocapPoseNode::UpdateInternal(const FAnimationUpdateContext& Context) {
  GetEvaluateGraphExposedInputs().Execute(Context);
  Super::UpdateInternal(Context);
}

void FRebocapPoseNode::Initialize_AnyThread(const FAnimationInitializeContext& Context) {
  GetEvaluateGraphExposedInputs().Execute(Context);
  Super::Initialize_AnyThread(Context);
}