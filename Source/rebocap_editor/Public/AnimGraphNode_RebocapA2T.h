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

    // --- 视口骨骼高亮显示设置 ---
    /** 是否在动画预览视口中实时高亮显示被 A2T 节点控制的骨骼与关节 */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Show Controlled Bones / 显示受控骨骼高亮"))
    bool bShowControlledBones = true;

    /** 受控关节点高亮颜色（默认亮青绿） */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Joint Color / 关节点颜色"))
    FLinearColor JointColor = FLinearColor(0.0f, 1.0f, 0.5f, 1.0f);

    /** 骨骼连接线高亮颜色（默认亮金黄） */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Link Color / 骨骼连线颜色"))
    FLinearColor LinkColor = FLinearColor(1.0f, 0.85f, 0.1f, 1.0f);

    /** 关节点球体半径 */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Joint Radius / 关节点半径", ClampMin = "0.5", ClampMax = "10.0"))
    float JointRadius = 2.2f;

    /** 骨骼连线粗细 */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Link Thickness / 连线粗细", ClampMin = "0.5", ClampMax = "8.0"))
    float LinkThickness = 1.6f;

    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FText GetTooltipText() const override;
    virtual FText GetMenuCategory() const override;
    virtual FText GetKeywords() const override;

    // 视口实时绘制受控骨骼高亮
    virtual void Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const override;

    // 实时同步节点数据至预览视口（无需点击编译即可所见即所得动态预览）
    virtual void CopyNodeDataToPreviewNode(FAnimNode_Base* InPreviewNode) override;

    // 属性修改回调：处理预设一键填充与对称镜像修改
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    // JSON 导出与导入
    void ExportToJson();
    void ImportFromJson();
};
