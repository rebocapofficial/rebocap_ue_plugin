#pragma once

#include "CoreMinimal.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "rebocap_a2t_node.generated.h"

/**
 * Rebocap 专属四肢与关节 A-Pose 转 T-Pose 姿态校准节点 (Runtime)
 * 支持对肩膀、大臂、小臂、手腕、大腿、小腿、脚部进行全方位的局部旋转累加与数值微调。
 */
USTRUCT(BlueprintInternalUseOnly)
struct REBOCAP_RUNTIME_API FAnimNode_RebocapA2T : public FAnimNode_SkeletalControlBase
{
    GENERATED_USTRUCT_BODY()

    FAnimNode_RebocapA2T();

    // ==========================================
    // --- 1. 左上肢 (Left Arm) ---
    // ==========================================

    /** 左锁骨/肩膀骨骼（默认 clavicle_l） */
    UPROPERTY(EditAnywhere, Category = "1. 左上肢 (Left Arm)", meta = (DisplayName = "左锁骨/肩膀 (Left Clavicle)"))
    FBoneReference LeftClavicle;

    /** 左锁骨/肩膀旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 左上肢 (Left Arm)", meta = (PinHiddenByDefault, DisplayName = "左锁骨旋转偏移"))
    FRotator LeftClavicleOffset;

    /** 左大臂骨骼（默认 upperarm_l） */
    UPROPERTY(EditAnywhere, Category = "1. 左上肢 (Left Arm)", meta = (DisplayName = "左大臂 (Left UpperArm)"))
    FBoneReference LeftUpperArm;

    /** 左大臂旋转偏移（默认 Roll/Z: +50° 抬手展开至 T-Pose） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 左上肢 (Left Arm)", meta = (PinHiddenByDefault, DisplayName = "左大臂旋转偏移"))
    FRotator LeftUpperArmOffset;

    /** 左小臂骨骼（默认 lowerarm_l） */
    UPROPERTY(EditAnywhere, Category = "1. 左上肢 (Left Arm)", meta = (DisplayName = "左小臂 (Left LowerArm)"))
    FBoneReference LeftLowerArm;

    /** 左小臂旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 左上肢 (Left Arm)", meta = (PinHiddenByDefault, DisplayName = "左小臂旋转偏移"))
    FRotator LeftLowerArmOffset;

    /** 左手/手腕骨骼（默认 hand_l） */
    UPROPERTY(EditAnywhere, Category = "1. 左上肢 (Left Arm)", meta = (DisplayName = "左手/手腕 (Left Hand)"))
    FBoneReference LeftHand;

    /** 左手/手腕旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. 左上肢 (Left Arm)", meta = (PinHiddenByDefault, DisplayName = "左手旋转偏移"))
    FRotator LeftHandOffset;

    // ==========================================
    // --- 2. 右上肢 (Right Arm) ---
    // ==========================================

    /** 右锁骨/肩膀骨骼（默认 clavicle_r） */
    UPROPERTY(EditAnywhere, Category = "2. 右上肢 (Right Arm)", meta = (DisplayName = "右锁骨/肩膀 (Right Clavicle)"))
    FBoneReference RightClavicle;

    /** 右锁骨/肩膀旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 右上肢 (Right Arm)", meta = (PinHiddenByDefault, DisplayName = "右锁骨旋转偏移"))
    FRotator RightClavicleOffset;

    /** 右大臂骨骼（默认 upperarm_r） */
    UPROPERTY(EditAnywhere, Category = "2. 右上肢 (Right Arm)", meta = (DisplayName = "右大臂 (Right UpperArm)"))
    FBoneReference RightUpperArm;

    /** 右大臂旋转偏移（默认 Roll/Z: -50° 抬手展开至 T-Pose） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 右上肢 (Right Arm)", meta = (PinHiddenByDefault, DisplayName = "右大臂旋转偏移"))
    FRotator RightUpperArmOffset;

    /** 右小臂骨骼（默认 lowerarm_r） */
    UPROPERTY(EditAnywhere, Category = "2. 右上肢 (Right Arm)", meta = (DisplayName = "右小臂 (Right LowerArm)"))
    FBoneReference RightLowerArm;

    /** 右小臂旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 右上肢 (Right Arm)", meta = (PinHiddenByDefault, DisplayName = "右小臂旋转偏移"))
    FRotator RightLowerArmOffset;

    /** 右手/手腕骨骼（默认 hand_r） */
    UPROPERTY(EditAnywhere, Category = "2. 右上肢 (Right Arm)", meta = (DisplayName = "右手/手腕 (Right Hand)"))
    FBoneReference RightHand;

    /** 右手/手腕旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. 右上肢 (Right Arm)", meta = (PinHiddenByDefault, DisplayName = "右手旋转偏移"))
    FRotator RightHandOffset;

    // ==========================================
    // --- 3. 左下肢 (Left Leg) ---
    // ==========================================

    /** 左大腿骨骼（默认 thigh_l） */
    UPROPERTY(EditAnywhere, Category = "3. 左下肢 (Left Leg)", meta = (DisplayName = "左大腿 (Left Thigh)"))
    FBoneReference LeftThigh;

    /** 左大腿旋转偏移（默认 Roll: -5° 修正收拢） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. 左下肢 (Left Leg)", meta = (PinHiddenByDefault, DisplayName = "左大腿旋转偏移"))
    FRotator LeftThighOffset;

    /** 左小腿骨骼（默认 calf_l） */
    UPROPERTY(EditAnywhere, Category = "3. 左下肢 (Left Leg)", meta = (DisplayName = "左小腿 (Left Calf)"))
    FBoneReference LeftCalf;

    /** 左小腿旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. 左下肢 (Left Leg)", meta = (PinHiddenByDefault, DisplayName = "左小腿旋转偏移"))
    FRotator LeftCalfOffset;

    /** 左脚/脚踝骨骼（默认 foot_l） */
    UPROPERTY(EditAnywhere, Category = "3. 左下肢 (Left Leg)", meta = (DisplayName = "左脚踝 (Left Foot)"))
    FBoneReference LeftFoot;

    /** 左脚/脚踝旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. 左下肢 (Left Leg)", meta = (PinHiddenByDefault, DisplayName = "左脚踝旋转偏移"))
    FRotator LeftFootOffset;

    // ==========================================
    // --- 4. 右下肢 (Right Leg) ---
    // ==========================================

    /** 右大腿骨骼（默认 thigh_r） */
    UPROPERTY(EditAnywhere, Category = "4. 右下肢 (Right Leg)", meta = (DisplayName = "右大腿 (Right Thigh)"))
    FBoneReference RightThigh;

    /** 右大腿旋转偏移（默认 Roll: +5° 修正收拢） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 右下肢 (Right Leg)", meta = (PinHiddenByDefault, DisplayName = "右大腿旋转偏移"))
    FRotator RightThighOffset;

    /** 右小腿骨骼（默认 calf_r） */
    UPROPERTY(EditAnywhere, Category = "4. 右下肢 (Right Leg)", meta = (DisplayName = "右小腿 (Right Calf)"))
    FBoneReference RightCalf;

    /** 右小腿旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 右下肢 (Right Leg)", meta = (PinHiddenByDefault, DisplayName = "右小腿旋转偏移"))
    FRotator RightCalfOffset;

    /** 右脚/脚踝骨骼（默认 foot_r） */
    UPROPERTY(EditAnywhere, Category = "4. 右下肢 (Right Leg)", meta = (DisplayName = "右脚踝 (Right Foot)"))
    FBoneReference RightFoot;

    /** 右脚/脚踝旋转偏移 (Roll/Pitch/Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. 右下肢 (Right Leg)", meta = (PinHiddenByDefault, DisplayName = "右脚踝旋转偏移"))
    FRotator RightFootOffset;

    // --- 虚函数重写 ---
    virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
    virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
    virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
};
