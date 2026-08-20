#include "rebocap_a2t_node.h"
#include "Animation/AnimInstanceProxy.h"

FAnimNode_RebocapA2T::FAnimNode_RebocapA2T()
    : LeftUpperArm(TEXT("upperarm_l"))
    , RightUpperArm(TEXT("upperarm_r"))
    , LeftThigh(TEXT("thigh_l"))
    , RightThigh(TEXT("thigh_r"))
    , LeftArmOffset(0.0f, 0.0f, 50.0f)     // Roll/Z +50 度抬手
    , RightArmOffset(0.0f, 0.0f, -50.0f)   // Roll/Z -50 度抬手
    , LeftThighOffset(0.0f, 0.0f, -5.0f)   // 双腿微调收拢
    , RightThighOffset(0.0f, 0.0f, 5.0f)   // 双腿微调收拢
    , Alpha(1.0f)
{
}

void FAnimNode_RebocapA2T::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
    LeftUpperArm.Initialize(RequiredBones);
    RightUpperArm.Initialize(RequiredBones);
    LeftThigh.Initialize(RequiredBones);
    RightThigh.Initialize(RequiredBones);
}

bool FAnimNode_RebocapA2T::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
    return Alpha > 0.0f && (LeftUpperArm.IsValidToEvaluate(RequiredBones) || RightUpperArm.IsValidToEvaluate(RequiredBones) || LeftThigh.IsValidToEvaluate(RequiredBones) || RightThigh.IsValidToEvaluate(RequiredBones));
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

    // 依序对左大臂、右大臂、左大腿、右大腿施加局部旋转
    ApplyAdditiveBoneRotation(LeftUpperArm, LeftArmOffset);
    ApplyAdditiveBoneRotation(RightUpperArm, RightArmOffset);
    ApplyAdditiveBoneRotation(LeftThigh, LeftThighOffset);
    ApplyAdditiveBoneRotation(RightThigh, RightThighOffset);
}
