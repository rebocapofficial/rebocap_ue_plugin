#include "AnimGraphNode_RebocapA2T.h"

#define LOCTEXT_NAMESPACE "RebocapA2TAnimGraphNode"

FText UAnimGraphNode_RebocapA2T::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("RebocapA2TNodeTitle", "Rebocap A-Pose to T-Pose");
}

FText UAnimGraphNode_RebocapA2T::GetTooltipText() const
{
    return LOCTEXT("RebocapA2TNodeTooltip", "搭配Rebocap的修正节点，用来修正a-pose角色的偏转。");
}

FText UAnimGraphNode_RebocapA2T::GetMenuCategory() const
{
    return LOCTEXT("RebocapA2TCategory", "Rebocap");
}

FText UAnimGraphNode_RebocapA2T::GetKeywords() const
{
    return LOCTEXT("RebocapA2TKeywords", "rebocap, a2t, tpose, a-pose, t-pose, calibrate, limb, mocap, live link");
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

    // 2. 对称修改：只要开启 bMirrorEdit，根据用户勾选的 XYZ 正负轴向规则自动镜像同步到右侧
    if (Node.bMirrorEdit)
    {
        const double MultRoll  = Node.bMirrorInvertRoll  ? -1.0 : 1.0;
        const double MultPitch = Node.bMirrorInvertPitch ? -1.0 : 1.0;
        const double MultYaw   = Node.bMirrorInvertYaw   ? -1.0 : 1.0;

        auto MirrorRot = [&](const FRotator& InRot) -> FRotator
        {
            return FRotator(InRot.Pitch * MultPitch, InRot.Yaw * MultYaw, InRot.Roll * MultRoll);
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
