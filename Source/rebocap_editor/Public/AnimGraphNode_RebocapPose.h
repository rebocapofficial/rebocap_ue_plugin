#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "rebocap_pose_node.h"
#include "LiveLinkTypes.h"
#include "LiveLinkRefSkeleton.h"
#include "AnimGraphNode_RebocapPose.generated.h"

UCLASS()
class REBOCAP_EDITOR_API UAnimGraphNode_RebocapPose : public UAnimGraphNode_SkeletalControlBase
{
    GENERATED_BODY()

public:
    // 运行时逻辑节点
    UPROPERTY(EditAnywhere, Category = Settings)
    FRebocapPoseNode Node;

    // 必须实现的虚函数 (UE5.1+ 需要加 const)
    virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }

    // 节点标题
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

    // 鼠标悬停提示 (Tooltip)
    virtual FText GetTooltipText() const override;

    // 节点分类
    virtual FText GetMenuCategory() const override;

    // 搜索关键词
    virtual FText GetKeywords() const override;
};