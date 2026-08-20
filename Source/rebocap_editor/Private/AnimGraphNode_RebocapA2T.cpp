#include "AnimGraphNode_RebocapA2T.h"

#define LOCTEXT_NAMESPACE "RebocapA2TAnimGraphNode"

FText UAnimGraphNode_RebocapA2T::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RebocapA2TNodeTitle", "Rebocap A-Pose to T-Pose (A2T 姿态校准)");
}

FText UAnimGraphNode_RebocapA2T::GetTooltipText() const
{
    return LOCTEXT("RebocapA2TNodeTooltip", "Rebocap 专属四肢姿态校准节点：将 UE5/MetaHuman/二次元等 A-Pose 角色的四肢一键展开为标准 T-Pose，以便完美对接 Rebocap 动捕。");
}

FText UAnimGraphNode_RebocapA2T::GetMenuCategory() const
{
    return LOCTEXT("RebocapA2TCategory", "Rebocap");
}

FText UAnimGraphNode_RebocapA2T::GetKeywords() const
{
    return LOCTEXT("RebocapA2TKeywords", "rebocap, a2t, tpose, a-pose, t-pose, calibrate, 校准, 姿态, 展开");
}

#undef LOCTEXT_NAMESPACE
