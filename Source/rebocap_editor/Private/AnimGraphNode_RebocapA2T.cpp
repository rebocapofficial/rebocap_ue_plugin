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
    const FName MemberName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

    // 1. 预设切换：自动应用对应角色的 A-Pose 旋转数据
    if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, PresetTemplate) ||
        MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, PresetTemplate))
    {
        if (Node.PresetTemplate != ERebocapA2TPreset::Custom)
        {
            Node.ApplyPreset(Node.PresetTemplate);
        }
    }
    // 2. 对称修改：修改左侧肢体自动镜像更新右侧肢体
    else if (Node.bMirrorEdit)
    {
        auto MirrorRot = [](const FRotator& InRot) -> FRotator
        {
            return FRotator(InRot.Pitch, -InRot.Yaw, -InRot.Roll);
        };

        if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftClavicleOffset) ||
            MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftClavicleOffset))
        {
            Node.RightClavicleOffset = MirrorRot(Node.LeftClavicleOffset);
        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftUpperArmOffset) ||
                 MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftUpperArmOffset))
        {
            Node.RightUpperArmOffset = MirrorRot(Node.LeftUpperArmOffset);
        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftLowerArmOffset) ||
                 MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftLowerArmOffset))
        {
            Node.RightLowerArmOffset = MirrorRot(Node.LeftLowerArmOffset);
        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftHandOffset) ||
                 MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftHandOffset))
        {
            Node.RightHandOffset = MirrorRot(Node.LeftHandOffset);
        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftThighOffset) ||
                 MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftThighOffset))
        {
            Node.RightThighOffset = MirrorRot(Node.LeftThighOffset);
        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftCalfOffset) ||
                 MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftCalfOffset))
        {
            Node.RightCalfOffset = MirrorRot(Node.LeftCalfOffset);
        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftFootOffset) ||
                 MemberName == GET_MEMBER_NAME_CHECKED(FAnimNode_RebocapA2T, LeftFootOffset))
        {
            Node.RightFootOffset = MirrorRot(Node.LeftFootOffset);
        }
    }
}

#undef LOCTEXT_NAMESPACE
