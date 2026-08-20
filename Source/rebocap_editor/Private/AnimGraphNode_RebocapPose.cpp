#include "AnimGraphNode_RebocapPose.h"

#define LOCTEXT_NAMESPACE "RebocapAnimGraphNode"


FText UAnimGraphNode_RebocapPose::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RebocapPoseNodeTitle", "Rebocap Body Pose");
}

FText UAnimGraphNode_RebocapPose::GetTooltipText() const
{
    return LOCTEXT("RebocapPoseNodeTooltip", "连接Rebocap的实时动捕，并且通过骨骼映射驱动角色。（FK动画）");
}

FText UAnimGraphNode_RebocapPose::GetMenuCategory() const
{
    return LOCTEXT("RebocapPoseCategory", "Rebocap");
}

FText UAnimGraphNode_RebocapPose::GetKeywords() const
{
    return LOCTEXT("RebocapPoseKeywords", "rebocap, mocap, live link, 动捕, 姿态, 骨骼");
}

#undef LOCTEXT_NAMESPACE
