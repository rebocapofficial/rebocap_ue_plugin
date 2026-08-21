#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "rebocap_a2t_node.h"
#include "AnimGraphNode_RebocapA2T.generated.h"

/**
 * Rebocap A2T 姿态校准动画图节点 (Editor - Local Space)
 * 一键将 A-Pose 角色的四肢按局部旋转偏移展开为标准 T-Pose，完美对接动捕。
 */
UCLASS()
class REBOCAP_EDITOR_API UAnimGraphNode_RebocapA2T : public UAnimGraphNode_Base
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = Settings)
    FAnimNode_RebocapA2T Node;

    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FText GetTooltipText() const override;
    virtual FText GetMenuCategory() const override;
    virtual FText GetKeywords() const override;

    // 实时同步节点数据至预览视口（无需点击编译即可所见即所得动态预览）
    virtual void CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode) override;

    // 属性修改回调：处理预设一键填充与对称镜像修改
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    // JSON 导出与导入
    void ExportToJson();
    void ImportFromJson();
};
