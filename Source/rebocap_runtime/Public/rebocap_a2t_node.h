#pragma once

#include "CoreMinimal.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "rebocap_a2t_node.generated.h"

/**
 * Rebocap 专属 A-Pose 转 T-Pose 四肢姿态校准节点 (Runtime)
 * 允许在单一节点中同时对左大臂、右大臂、左大腿、右大腿进行局部空间旋转累加，完美将 A-Pose 展开为标准 T-Pose。
 */
USTRUCT(BlueprintInternalUseOnly)
struct REBOCAP_RUNTIME_API FAnimNode_RebocapA2T : public FAnimNode_SkeletalControlBase
{
    GENERATED_USTRUCT_BODY()

    FAnimNode_RebocapA2T();

    // --- 1. 骨骼指定 (Bone References) ---

    /** 左大臂骨骼名称（默认 upperarm_l） */
    UPROPERTY(EditAnywhere, Category = "1. 骨骼指定 (Bones)", meta = (DisplayName = "Left UpperArm Bone (左大臂骨骼)"))
    FBoneReference LeftUpperArm;

    /** 右大臂骨骼名称（默认 upperarm_r） */
    UPROPERTY(EditAnywhere, Category = "1. 骨骼指定 (Bones)", meta = (DisplayName = "Right UpperArm Bone (右大臂骨骼)"))
    FBoneReference RightUpperArm;

    /** 左大腿骨骼名称（默认 thigh_l） */
    UPROPERTY(EditAnywhere, Category = "1. 骨骼指定 (Bones)", meta = (DisplayName = "Left Thigh Bone (左大腿骨骼)"))
    FBoneReference LeftThigh;

    /** 右大腿骨骼名称（默认 thigh_r） */
    UPROPERTY(EditAnywhere, Category = "1. 骨骼指定 (Bones)", meta = (DisplayName = "Right Thigh Bone (右大腿骨骼)"))
    FBoneReference RightThigh;

    // --- 2. 四肢旋转偏移 (Rotation Offsets) ---

    /** 左大臂局部旋转偏移（默认 Roll/Z: +50° 抬起手臂） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 旋转偏移 (Offsets)", meta = (PinShownByDefault, DisplayName = "Left Arm Offset (左大臂旋转偏移)"))
    FRotator LeftArmOffset;

    /** 右大臂局部旋转偏移（默认 Roll/Z: -50° 抬起手臂） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 旋转偏移 (Offsets)", meta = (PinShownByDefault, DisplayName = "Right Arm Offset (右大臂旋转偏移)"))
    FRotator RightArmOffset;

    /** 左大腿局部旋转偏移（默认微调并拢双腿） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 旋转偏移 (Offsets)", meta = (PinShownByDefault, DisplayName = "Left Thigh Offset (左大腿旋转偏移)"))
    FRotator LeftThighOffset;

    /** 右大腿局部旋转偏移（默认微调并拢双腿） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 旋转偏移 (Offsets)", meta = (PinShownByDefault, DisplayName = "Right Thigh Offset (右大腿旋转偏移)"))
    FRotator RightThighOffset;

    // --- 虚函数重写 ---
    virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
    virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
    virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
};
