#include "AnimGraphNode_RebocapPose.h"

#define LOCTEXT_NAMESPACE "RebocapAnimGraphNode"


FText UAnimGraphNode_RebocapPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RebocapPoseNodeTitle", "Rebocap Body Pose");
}

FText UAnimGraphNode_RebocapPose::GetTooltipText() const
{
    return LOCTEXT("RebocapPoseNodeTooltip", "Drives skeletal mesh with real-time Rebocap LiveLink motion capture data via bone retarget mapping.");
}

FText UAnimGraphNode_RebocapPose::GetMenuCategory() const
{
    return LOCTEXT("RebocapPoseCategory", "Rebocap");
}

FText UAnimGraphNode_RebocapPose::GetKeywords() const
{
    return LOCTEXT("RebocapPoseKeywords", "rebocap, mocap, live link, pose, skeleton, motion capture");
}

#undef LOCTEXT_NAMESPACE
