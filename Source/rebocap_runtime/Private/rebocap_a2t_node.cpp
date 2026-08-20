#include "rebocap_a2t_node.h"
#include "Animation/AnimInstanceProxy.h"

FAnimNode_RebocapA2T::FAnimNode_RebocapA2T()
    // 1. 左上肢 (Left Arm)
    : LeftClavicle(TEXT("clavicle_l"))
    , LeftClavicleOffset(FRotator::ZeroRotator)
    , LeftUpperArm(TEXT("upperarm_l"))
    , LeftUpperArmOffset(0.0f, 0.0f, 50.0f) // Roll/Z +50 抬手
    , LeftLowerArm(TEXT("lowerarm_l"))
    , LeftLowerArmOffset(FRotator::ZeroRotator)
    , LeftHand(TEXT("hand_l"))
    , LeftHandOffset(FRotator::ZeroRotator)

    // 2. 右上肢 (Right Arm)
    , RightClavicle(TEXT("clavicle_r"))
    , RightClavicleOffset(FRotator::ZeroRotator)
    , RightUpperArm(TEXT("upperarm_r"))
    , RightUpperArmOffset(0.0f, 0.0f, -50.0f) // Roll/Z -50 抬手
    , RightLowerArm(TEXT("lowerarm_r"))
    , RightLowerArmOffset(FRotator::ZeroRotator)
    , RightHand(TEXT("hand_r"))
    , RightHandOffset(FRotator::ZeroRotator)

    // 3. 左下肢 (Left Leg)
    , LeftThigh(TEXT("thigh_l"))
    , LeftThighOffset(0.0f, 0.0f, -5.0f) // 双腿微调收拢
    , LeftCalf(TEXT("calf_l"))
    , LeftCalfOffset(FRotator::ZeroRotator)
    , LeftFoot(TEXT("foot_l"))
    , LeftFootOffset(FRotator::ZeroRotator)

    // 4. 右下肢 (Right Leg)
    , RightThigh(TEXT("thigh_r"))
    , RightThighOffset(0.0f, 0.0f, 5.0f) // 双腿微调收拢
    , RightCalf(TEXT("calf_r"))
    , RightCalfOffset(FRotator::ZeroRotator)
    , RightFoot(TEXT("foot_r"))
    , RightFootOffset(FRotator::ZeroRotator)
{
    Alpha = 1.0f;
}

void FAnimNode_RebocapA2T::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
    LeftClavicle.Initialize(RequiredBones);
    LeftUpperArm.Initialize(RequiredBones);
    LeftLowerArm.Initialize(RequiredBones);
    LeftHand.Initialize(RequiredBones);

    RightClavicle.Initialize(RequiredBones);
    RightUpperArm.Initialize(RequiredBones);
    RightLowerArm.Initialize(RequiredBones);
    RightHand.Initialize(RequiredBones);

    LeftThigh.Initialize(RequiredBones);
    LeftCalf.Initialize(RequiredBones);
    LeftFoot.Initialize(RequiredBones);

    RightThigh.Initialize(RequiredBones);
    RightCalf.Initialize(RequiredBones);
    RightFoot.Initialize(RequiredBones);
}

bool FAnimNode_RebocapA2T::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
    return Alpha > 0.0f && (
        LeftUpperArm.IsValidToEvaluate(RequiredBones) ||
        RightUpperArm.IsValidToEvaluate(RequiredBones) ||
        LeftThigh.IsValidToEvaluate(RequiredBones) ||
        RightThigh.IsValidToEvaluate(RequiredBones)
    );
}

void FAnimNode_RebocapA2T::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
    check(OutBoneTransforms.Num() == 0);
    if (FMath::IsNearlyZero(Alpha))
    {
        return;
    }

    const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

    auto ApplyAdditiveBoneRotation = [&](FBoneReference& BoneRef, const FRotator& OffsetRotator)
    {
        if (BoneRef.BoneIndex == INDEX_NONE || OffsetRotator.IsNearlyZero())
        {
            return;
        }

        const FCompactPoseBoneIndex CompactIndex = BoneRef.GetCompactPoseIndex(BoneContainer);
        if (CompactIndex != INDEX_NONE)
        {
            FTransform CSTransform = Output.Pose.GetComponentSpaceTransform(CompactIndex);
            
            // 计算局部骨骼空间的附加四元数
            const FQuat LocalAdditiveQuat = FQuat(OffsetRotator * Alpha);
            
            // 在局部骨骼坐标系中应用旋转 (CSTransform.Rotation * LocalAdditiveQuat)
            const FQuat NewCSRotation = CSTransform.GetRotation() * LocalAdditiveQuat;
            CSTransform.SetRotation(NewCSRotation);

            Output.Pose.SetComponentSpaceTransform(CompactIndex, CSTransform);
        }
    };

    // 1. 左上肢 (Left Arm)
    ApplyAdditiveBoneRotation(LeftClavicle, LeftClavicleOffset);
    ApplyAdditiveBoneRotation(LeftUpperArm, LeftUpperArmOffset);
    ApplyAdditiveBoneRotation(LeftLowerArm, LeftLowerArmOffset);
    ApplyAdditiveBoneRotation(LeftHand, LeftHandOffset);

    // 2. 右上肢 (Right Arm)
    ApplyAdditiveBoneRotation(RightClavicle, RightClavicleOffset);
    ApplyAdditiveBoneRotation(RightUpperArm, RightUpperArmOffset);
    ApplyAdditiveBoneRotation(RightLowerArm, RightLowerArmOffset);
    ApplyAdditiveBoneRotation(RightHand, RightHandOffset);

    // 3. 左下肢 (Left Leg)
    ApplyAdditiveBoneRotation(LeftThigh, LeftThighOffset);
    ApplyAdditiveBoneRotation(LeftCalf, LeftCalfOffset);
    ApplyAdditiveBoneRotation(LeftFoot, LeftFootOffset);

    // 4. 右下肢 (Right Leg)
    ApplyAdditiveBoneRotation(RightThigh, RightThighOffset);
    ApplyAdditiveBoneRotation(RightCalf, RightCalfOffset);
    ApplyAdditiveBoneRotation(RightFoot, RightFootOffset);
}
