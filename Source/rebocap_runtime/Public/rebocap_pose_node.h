#pragma once

#include <optional>

#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNodeBase.h"
#include "ILiveLinkClient.h"
#include "Runtime/AnimGraphRuntime/Public/BoneControllers/AnimNode_SkeletalControlBase.h"
#include "Runtime/LiveLinkAnimationCore/Public/LiveLinkRemapAsset.h"
#include "rebocap_body_remap_asset.h"
#include "rebocap_pose_node.generated.h"

// 1. 骨骼定义
USTRUCT()
struct REBOCAP_RUNTIME_API FRebocapBodyMap {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference Pelvis;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Hip;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Hip;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference Spine1;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Knee;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Knee;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference Spine2;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Ankle;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Ankle;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference Spine3;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Foot;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Foot;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference Neck;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Collar;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Collar;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference Head;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Shoulder;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Shoulder;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Elbow;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Elbow;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Wrist;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Wrist;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference L_Hand;
  UPROPERTY(EditAnywhere, Category = SkeletalControl) FBoneReference R_Hand;
};

// 2. TPose
struct TPose {
  std::optional<FTransform> pelvis;
  std::optional<FTransform> l_hip;
  std::optional<FTransform> r_hip;
  std::optional<FTransform> spine1;
  std::optional<FTransform> l_knee;
  std::optional<FTransform> r_knee;
  std::optional<FTransform> spine2;
  std::optional<FTransform> l_ankle;
  std::optional<FTransform> r_ankle;
  std::optional<FTransform> spine3;
  std::optional<FTransform> l_foot;
  std::optional<FTransform> r_foot;
  std::optional<FTransform> neck;
  std::optional<FTransform> l_collar;
  std::optional<FTransform> r_collar;
  std::optional<FTransform> head;
  std::optional<FTransform> l_shoulder;
  std::optional<FTransform> r_shoulder;
  std::optional<FTransform> l_elbow;
  std::optional<FTransform> r_elbow;
  std::optional<FTransform> l_wrist;
  std::optional<FTransform> r_wrist;
  std::optional<FTransform> l_hand;
  std::optional<FTransform> r_hand;
};

// 3. 动画节点
USTRUCT()
struct REBOCAP_RUNTIME_API FRebocapPoseNode final : public FAnimNode_SkeletalControlBase {
  GENERATED_USTRUCT_BODY()

  FRebocapPoseNode();

  UPROPERTY()
  FRebocapBodyMap bone_map_;

  /** Live Link 的主题名称（默认使用 rebocap）。 */
  UPROPERTY(EditAnywhere, Category = SourceData, meta = (PinHiddenByDefault, ToolTip = "Live Link 的主题名称（默认使用 rebocap）。"))
  FLiveLinkSubjectName LiveLinkSubjectName;

  // --- 全权交给蓝图控制的开关 ---
  
  // 1. 是否自动回传 T-Pose
  /** 
   * 是否自动把当前角色的T-Pose骨骼数据发回到rebocap里，
   * 不想反复被数据刷屏或已经制作有固定的骨架文件后可以关闭该功能；
   * 建议创建功能引脚去手动控制。（默认开启）
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rebocap Settings", meta = (PinShownByDefault, DisplayName="Auto submit skeleton data", ToolTip = "是否自动把当前角色的T-Pose骨骼数据发回到rebocap里，\n不想反复被数据刷屏或已经制作有固定的骨架文件后可以关闭该功能；\n建议创建功能引脚去手动控制。（默认开启）"))
  bool bAutoSkeleton = true;

  // 2. 是否自动连接 (默认开启)
  /** 控制是否连接Rebocap。（默认开启） */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rebocap Settings", meta = (PinShownByDefault, DisplayName = "Connect Rebocap", ToolTip = "控制是否连接Rebocap。（默认开启）"))
  bool bAutoConnect = true;

  // -----------------------------

  UPROPERTY(transient)
  ULiveLinkRemapAsset* current_retarget_asset_ = nullptr;

  /** 填写骨骼映射/骨骼重定向对应名字映射表。 */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = Retarget, meta = (PinShownByDefault, ToolTip = "填写骨骼映射/骨骼重定向对应名字映射表。"))
  TSubclassOf<URebocapMapData> retarget_asset_;

  virtual void OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance) override;
  virtual void GatherDebugData(FNodeDebugData& DebugData) override;

  void Init_Foot_Vertices_And_SkeletalData(USkeletalMeshComponent* MeshComp);
  virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;

  virtual bool HasPreUpdate() const override final { return true; }
  virtual void PreUpdate(const UAnimInstance* InAnimInstance) override final;

  void InitBoneMap();

  virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override { return true; }
  virtual void UpdateComponentPose_AnyThread(const FAnimationUpdateContext& Context) override;
  virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
  virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

 private:
  ILiveLinkClient* live_link_client_ = nullptr;
  virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
  
  // 状态管理函数 (移除了参数)
  void UpdateConnectionState();

  TPose t_pose_;
  bool init_vertices_ = false;
  
  TArray<FVector3f> LeftVertices_, LeftNormals_, RightVertices_, RightNormals_, SkeletonPosition_;
};