#include "rebocap_pose.h"

#include "rebocap_skeleton_data.h"
#include "Kismet2/CompilerResultsLog.h"

#define LOCTEXT_NAMESPACE "RebocapPose"

void URebocapPose::check_for_warnings(FName name, FBoneReference bone, USkeleton* for_skeleton, FCompilerResultsLog& message_log) {
  if (for_skeleton->GetReferenceSkeleton().FindBoneIndex(bone.BoneName) == INDEX_NONE) {
    if (bone.BoneName == NAME_None) {
      FFormatNamedArguments args;
      args.Add(TEXT("BoneName"), FText::FromName(name));
      FText msg = FText::Format(LOCTEXT("NoBoneFoundToModify", "@@ - Bone {BoneName} override in the bone override map is not set"), args);
      message_log.Warning(*msg.ToString(), this);
    } else {
      FFormatNamedArguments args;
      args.Add(TEXT("BoneName"), FText::FromName(name));
      args.Add(TEXT("OverrideName"), FText::FromName(bone.BoneName));
      FText msg = FText::Format(
          LOCTEXT("NoBoneFoundToModify",
                  "@@ - Bone override of \"{OverrideName}\" for \"{BoneName}\" in the bone override map wasn't found in the skeleton"),
          args);
      message_log.Warning(*msg.ToString(), this);
    }
  }
}

FText URebocapPose::GetNodeTitle(ENodeTitleType::Type TitleType) const { return GetControllerDescription(); }

FText URebocapPose::GetTooltipText() const {
  return LOCTEXT("RebocapPose_Tooltip", "This node will make the character pose the same as the live feed from the defined Rebocap.");
}

FText URebocapPose::GetControllerDescription() const { return LOCTEXT("Rebocap Pose", "Rebocap Body Pose"); }

void URebocapPose::ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog) {
  Super::ValidateAnimNodeDuringCompilation(ForSkeleton, MessageLog);
  UClass* retarget_asset_ptr = node_.retarget_asset_.Get();
  bool asset_is_retargetted = false;
  if (IsValid(retarget_asset_ptr)) {
    FString Msg = "retarget asset: " + node_.retarget_asset_->GetName();
    MessageLog.Note(*Msg, this);
    node_.current_retarget_asset_ = NewObject<ULiveLinkRemapAsset>(GetTransientPackage(), retarget_asset_ptr);
    node_.current_retarget_asset_->Initialize();
    asset_is_retargetted = true;
  } else {
    FString msg = "could not get retarget asset!";
    MessageLog.Warning(*msg, this);
  }
  bool check_bone_overrides = true;
  if (asset_is_retargetted && node_.current_retarget_asset_->IsValidLowLevel()) {
    node_.InitBoneMap();
  } else {
    FString msg = "Bone map override appears to not be set.  If is set, try recompiling";
    MessageLog.Warning(*msg, this);
    check_bone_overrides = false;
  }

  if (check_bone_overrides && ForSkeleton && !ForSkeleton->HasAnyFlags(RF_NeedPostLoad)) {
    check_for_warnings(rebocap_bones::pelvis, node_.bone_map_.Pelvis, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_hip, node_.bone_map_.L_Hip, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_hip, node_.bone_map_.R_Hip, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::spine1, node_.bone_map_.Spine1, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_knee, node_.bone_map_.L_Knee, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_knee, node_.bone_map_.R_Knee, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::spine2, node_.bone_map_.Spine2, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_ankle, node_.bone_map_.L_Ankle, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_ankle, node_.bone_map_.R_Ankle, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::spine3, node_.bone_map_.Spine3, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_foot, node_.bone_map_.L_Foot, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_foot, node_.bone_map_.R_Foot, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::neck, node_.bone_map_.Neck, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_collar, node_.bone_map_.L_Collar, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_collar, node_.bone_map_.R_Collar, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::head, node_.bone_map_.Head, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_shoulder, node_.bone_map_.L_Shoulder, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_shoulder, node_.bone_map_.R_Shoulder, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_elbow, node_.bone_map_.L_Elbow, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_elbow, node_.bone_map_.R_Elbow, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_wrist, node_.bone_map_.L_Wrist, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_wrist, node_.bone_map_.R_Wrist, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::l_hand, node_.bone_map_.L_Hand, ForSkeleton, MessageLog);
    check_for_warnings(rebocap_bones::r_hand, node_.bone_map_.R_Hand, ForSkeleton, MessageLog);
  }

  MessageLog.Note(*LOCTEXT("UsageNote",
                           "@@ - For this animation node to work properly the character needs to be in T-pose when this node is evaluating "
                           "the pose and all bones in Bone Map need to be specified. This node will change the body position of the "
                           "character to match that of the Smartsuit, assuming the character is in TPose, which means it will set an "
                           "absolute position of the character. If you want to change the position of the character is better to do it "
                           "after this node. The alpha value of this node is ignored at the moment.")
                       .ToString(),
                  this);
}

void URebocapPose::CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode) {
  FRebocapPoseNode* modify_bone = static_cast<FRebocapPoseNode*>(InPreviewNode);
  node_.retarget_asset_ = modify_bone->retarget_asset_;
}

void URebocapPose::CopyPinDefaultsToNodeData(UEdGraphPin* InPin) {}

void URebocapPose::PreloadRequiredAssets() {
  Super::PreloadRequiredAssets();
  PreloadObject(node_.retarget_asset_);
}

#undef LOCTEXT_NAMESPACE