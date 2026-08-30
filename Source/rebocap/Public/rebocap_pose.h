#pragma once

#include "Runtime/Launch/Resources/Version.h"
#if ENGINE_MAJOR_VERSION >= 5
#include "Editor/AnimGraph/Public/AnimGraphNode_SkeletalControlBase.h"
#else
#include "AnimGraphNode_SkeletalControlBase.h"
#endif

#include "rebocap_pose_node.h"
// clang-format off
#include "rebocap_pose.generated.h"
// clang-format on

UCLASS(meta = (Keywords = "Rebocap Pose"))
class REBOCAP_API URebocapPose final : public UAnimGraphNode_SkeletalControlBase {
  GENERATED_BODY()
  UPROPERTY(EditAnywhere, Category = Settings)
  FRebocapPoseNode node_;

 private:
  FText GetNodeTitle(ENodeTitleType::Type TitleType) const final;
  FText GetTooltipText() const final;
  FText GetControllerDescription() const final;
  void ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog) final;
  void CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode) final;
  void CopyPinDefaultsToNodeData(UEdGraphPin* InPin) final;
  void PreloadRequiredAssets() final;
  const FAnimNode_SkeletalControlBase* GetNode() const final { return &node_; }
  void check_for_warnings(FName name, FBoneReference bone, USkeleton* for_skeleton, FCompilerResultsLog& message_log);
};