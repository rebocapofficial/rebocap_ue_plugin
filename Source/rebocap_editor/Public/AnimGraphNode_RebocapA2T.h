#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "rebocap_a2t_node.h"
#include "AnimGraphNode_RebocapA2T.generated.h"

/**
 * Rebocap A2T 姿态校准动画图节点 (Editor)
 * 一键将 A-Pose 角色的四肢按局部旋转偏移展开为标准 T-Pose，完美对接动捕。
 */
UCLASS()
class REBOCAP_EDITOR_API UAnimGraphNode_RebocapA2T : public UAnimGraphNode_SkeletalControlBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = Settings)
    FAnimNode_RebocapA2T Node;

    virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }

    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FText GetTooltipText() const override;
    virtual FText GetMenuCategory() const override;
    virtual FText GetKeywords() const override;

    // 实时同步节点数据至预览视口（无需点击编译即可所见即所得动态预览）
    virtual void CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode) override;
};
