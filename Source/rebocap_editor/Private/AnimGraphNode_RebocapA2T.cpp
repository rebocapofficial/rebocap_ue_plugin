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

void UAnimGraphNode_RebocapA2T::CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode)
{
    FAnimNode_RebocapA2T* PreviewNode = static_cast<FAnimNode_RebocapA2T*>(InPreviewNode);

    PreviewNode->LeftClavicleOffset = Node.LeftClavicleOffset;
    PreviewNode->LeftUpperArmOffset = Node.LeftUpperArmOffset;
    PreviewNode->LeftLowerArmOffset = Node.LeftLowerArmOffset;
    PreviewNode->LeftHandOffset = Node.LeftHandOffset;

    PreviewNode->RightClavicleOffset = Node.RightClavicleOffset;
    PreviewNode->RightUpperArmOffset = Node.RightUpperArmOffset;
    PreviewNode->RightLowerArmOffset = Node.RightLowerArmOffset;
    PreviewNode->RightHandOffset = Node.RightHandOffset;

    PreviewNode->LeftThighOffset = Node.LeftThighOffset;
    PreviewNode->LeftCalfOffset = Node.LeftCalfOffset;
    PreviewNode->LeftFootOffset = Node.LeftFootOffset;

    PreviewNode->RightThighOffset = Node.RightThighOffset;
    PreviewNode->RightCalfOffset = Node.RightCalfOffset;
    PreviewNode->RightFootOffset = Node.RightFootOffset;

    PreviewNode->Alpha = Node.Alpha;
}

void UAnimGraphNode_RebocapA2T::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    // 1. 预设切换：自动应用对应角色的 A-Pose 旋转数据
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, PresetTemplate))
    {
        if (Node.PresetTemplate != ERebocapA2TPreset::Custom)
        {
            Node.ApplyPreset(Node.PresetTemplate);
        }
        return;
    }

    // 2. 对称修改：只要开启 bMirrorEdit，修改左侧任意分量立即全自动镜像同步到右侧
    if (Node.bMirrorEdit)
    {
        auto MirrorRot = [](const FRotator& InRot) -> FRotator
        {
            return FRotator(InRot.Pitch, -InRot.Yaw, -InRot.Roll);
        };

        Node.RightClavicleOffset = MirrorRot(Node.LeftClavicleOffset);
        Node.RightUpperArmOffset = MirrorRot(Node.LeftUpperArmOffset);
        Node.RightLowerArmOffset = MirrorRot(Node.LeftLowerArmOffset);
        Node.RightHandOffset     = MirrorRot(Node.LeftHandOffset);

        Node.RightThighOffset    = MirrorRot(Node.LeftThighOffset);
        Node.RightCalfOffset     = MirrorRot(Node.LeftCalfOffset);
        Node.RightFootOffset     = MirrorRot(Node.LeftFootOffset);
    }
}

#undef LOCTEXT_NAMESPACE
