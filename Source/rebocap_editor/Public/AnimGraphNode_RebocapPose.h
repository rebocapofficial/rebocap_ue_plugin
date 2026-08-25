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

    // --- 视口骨骼高亮显示设置 ---
    /** 是否在动画预览视口中实时高亮显示被 Rebocap 动捕驱动的骨骼与关节 */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Show Controlled Bones / 显示受控骨骼高亮"))
    bool bShowControlledBones = true;

    /** 受控关节点高亮颜色（默认亮霓虹蓝） */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Joint Color / 关节点颜色"))
    FLinearColor JointColor = FLinearColor(0.0f, 0.75f, 1.0f, 1.0f);

    /** 骨骼连接线高亮颜色（默认亮品红/洋红） */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Link Color / 骨骼连线颜色"))
    FLinearColor LinkColor = FLinearColor(1.0f, 0.2f, 0.6f, 1.0f);

    /** 关节点球体半径 */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Joint Radius / 关节点半径", ClampMin = "0.5", ClampMax = "10.0"))
    float JointRadius = 2.2f;

    /** 骨骼连线粗细 */
    UPROPERTY(EditAnywhere, Category = "Visual Debug / 视口骨骼高亮", meta = (DisplayName = "Link Thickness / 连线粗细", ClampMin = "0.5", ClampMax = "8.0"))
    float LinkThickness = 1.6f;

    // 视口实时绘制受控骨骼高亮
    virtual void Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const override;

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