#include "rebocap_a2t_node.h"
#include "Animation/AnimInstanceProxy.h"

FAnimNode_RebocapA2T::FAnimNode_RebocapA2T()
    : PresetTemplate(ERebocapA2TPreset::UE5_Manny_Quinn)
    , bMirrorEdit(true)
    , bMirrorInvertRoll(false)
    , bMirrorInvertPitch(false)
    , bMirrorInvertYaw(false)

    // 1. 左上肢 (Left Arm)
    , LeftClavicle(TEXT("clavicle_l"))
    , LeftClavicleOffset(15.0f, 0.0f, 0.0f)   // Pitch 15°
    , LeftUpperArm(TEXT("upperarm_l"))
    , LeftUpperArmOffset(40.0f, 0.0f, 0.0f)   // Pitch 40° 抬手
    , LeftLowerArm(TEXT("lowerarm_l"))
    , LeftLowerArmOffset(0.0f, -40.0f, 0.0f)  // Yaw -40°
    , LeftHand(TEXT("hand_l"))
    , LeftHandOffset(FRotator::ZeroRotator)

    // 2. 右上肢 (Right Arm)
    , RightClavicle(TEXT("clavicle_r"))
    , RightClavicleOffset(15.0f, 0.0f, 0.0f)
    , RightUpperArm(TEXT("upperarm_r"))
    , RightUpperArmOffset(40.0f, 0.0f, 0.0f)
    , RightLowerArm(TEXT("lowerarm_r"))
    , RightLowerArmOffset(0.0f, -40.0f, 0.0f)
    , RightHand(TEXT("hand_r"))
    , RightHandOffset(FRotator::ZeroRotator)

    // 3. 左下肢 (Left Leg)
    , LeftThigh(TEXT("thigh_l"))
    , LeftThighOffset(-3.0f, 0.0f, -5.0f)     // Roll -5°, Pitch -3°
    , LeftCalf(TEXT("calf_l"))
    , LeftCalfOffset(FRotator::ZeroRotator)
    , LeftFoot(TEXT("foot_l"))
    , LeftFootOffset(FRotator::ZeroRotator)

    // 4. 右下肢 (Right Leg)
    , RightThigh(TEXT("thigh_r"))
    , RightThighOffset(-3.0f, 0.0f, -5.0f)
    , RightCalf(TEXT("calf_r"))
    , RightCalfOffset(FRotator::ZeroRotator)
    , RightFoot(TEXT("foot_r"))
    , RightFootOffset(FRotator::ZeroRotator)

    // 5. 躯干与头部 (Spine & Torso & Head)
    , Pelvis(TEXT("pelvis"))
    , PelvisOffset(FRotator::ZeroRotator)
    , Spine(TEXT("spine_02"))
    , SpineOffset(FRotator::ZeroRotator)
    , Chest(TEXT("spine_04"))
    , ChestOffset(FRotator::ZeroRotator)
    , UpChest(TEXT("spine_05"))
    , UpChestOffset(FRotator::ZeroRotator)
    , Neck(TEXT("neck_01"))
    , NeckOffset(FRotator::ZeroRotator)
    , Head(TEXT("head"))
    , HeadOffset(FRotator::ZeroRotator)
    , Alpha(1.0f)
{
}

void FAnimNode_RebocapA2T::ApplyPreset(ERebocapA2TPreset InPreset)
{
    PresetTemplate = InPreset;

    switch (InPreset)
    {
    case ERebocapA2TPreset::UE5_Manny_Quinn:
        bMirrorInvertRoll = false;
        bMirrorInvertPitch = false;
        bMirrorInvertYaw = false;

        LeftClavicleOffset = FRotator(15.0f, 0.0f, 0.0f);
        LeftUpperArmOffset = FRotator(40.0f, 0.0f, 0.0f);
        LeftLowerArmOffset = FRotator(0.0f, -40.0f, 0.0f);
        LeftHandOffset     = FRotator::ZeroRotator;

        RightClavicleOffset = FRotator(15.0f, 0.0f, 0.0f);
        RightUpperArmOffset = FRotator(40.0f, 0.0f, 0.0f);
        RightLowerArmOffset = FRotator(0.0f, -40.0f, 0.0f);
        RightHandOffset     = FRotator::ZeroRotator;

        LeftThighOffset = FRotator(-3.0f, 0.0f, -5.0f);
        LeftCalfOffset  = FRotator::ZeroRotator;
        LeftFootOffset  = FRotator::ZeroRotator;

        RightThighOffset = FRotator(-3.0f, 0.0f, -5.0f);
        RightCalfOffset  = FRotator::ZeroRotator;
        RightFootOffset  = FRotator::ZeroRotator;

        PelvisOffset     = FRotator::ZeroRotator;
        SpineOffset      = FRotator::ZeroRotator;
        ChestOffset      = FRotator::ZeroRotator;
        UpChestOffset    = FRotator::ZeroRotator;
        NeckOffset       = FRotator::ZeroRotator;
        HeadOffset       = FRotator::ZeroRotator;
        break;

    case ERebocapA2TPreset::MMD_Standard:
        bMirrorInvertRoll = false;
        bMirrorInvertPitch = false;
        bMirrorInvertYaw = false;

        LeftClavicleOffset = FRotator::ZeroRotator;
        LeftUpperArmOffset = FRotator(35.0f, 0.0f, 0.0f);
        LeftLowerArmOffset = FRotator::ZeroRotator;
        LeftHandOffset     = FRotator::ZeroRotator;

        RightClavicleOffset = FRotator::ZeroRotator;
        RightUpperArmOffset = FRotator(35.0f, 0.0f, 0.0f);
        RightLowerArmOffset = FRotator::ZeroRotator;
        RightHandOffset     = FRotator::ZeroRotator;

        LeftThighOffset = FRotator::ZeroRotator;
        LeftCalfOffset  = FRotator::ZeroRotator;
        LeftFootOffset  = FRotator::ZeroRotator;

        RightThighOffset = FRotator::ZeroRotator;
        RightCalfOffset  = FRotator::ZeroRotator;
        RightFootOffset  = FRotator::ZeroRotator;

        PelvisOffset     = FRotator::ZeroRotator;
        SpineOffset      = FRotator::ZeroRotator;
        ChestOffset      = FRotator::ZeroRotator;
        UpChestOffset    = FRotator::ZeroRotator;
        NeckOffset       = FRotator::ZeroRotator;
        HeadOffset       = FRotator::ZeroRotator;
        break;

    case ERebocapA2TPreset::VRoid_VRM:
        bMirrorInvertRoll = false;
        bMirrorInvertPitch = false;
        bMirrorInvertYaw = false;

        LeftClavicleOffset = FRotator::ZeroRotator;
        LeftUpperArmOffset = FRotator(35.0f, 0.0f, 0.0f);
        LeftLowerArmOffset = FRotator::ZeroRotator;
        LeftHandOffset     = FRotator::ZeroRotator;

        RightClavicleOffset = FRotator::ZeroRotator;
        RightUpperArmOffset = FRotator(35.0f, 0.0f, 0.0f);
        RightLowerArmOffset = FRotator::ZeroRotator;
        RightHandOffset     = FRotator::ZeroRotator;

        LeftThighOffset = FRotator::ZeroRotator;
        LeftCalfOffset  = FRotator::ZeroRotator;
        LeftFootOffset  = FRotator::ZeroRotator;

        RightThighOffset = FRotator::ZeroRotator;
        RightCalfOffset  = FRotator::ZeroRotator;
        RightFootOffset  = FRotator::ZeroRotator;

        PelvisOffset     = FRotator::ZeroRotator;
        SpineOffset      = FRotator::ZeroRotator;
        ChestOffset      = FRotator::ZeroRotator;
        UpChestOffset    = FRotator::ZeroRotator;
        NeckOffset       = FRotator::ZeroRotator;
        HeadOffset       = FRotator::ZeroRotator;
        break;

    case ERebocapA2TPreset::Mixamo_APose:
        bMirrorInvertRoll = false;
        bMirrorInvertPitch = false;
        bMirrorInvertYaw = false;

        LeftClavicleOffset = FRotator::ZeroRotator;
        LeftUpperArmOffset = FRotator(40.0f, 0.0f, 0.0f);
        LeftLowerArmOffset = FRotator::ZeroRotator;
        LeftHandOffset     = FRotator::ZeroRotator;

        RightClavicleOffset = FRotator::ZeroRotator;
        RightUpperArmOffset = FRotator(40.0f, 0.0f, 0.0f);
        RightLowerArmOffset = FRotator::ZeroRotator;
        RightHandOffset     = FRotator::ZeroRotator;

        LeftThighOffset = FRotator::ZeroRotator;
        LeftCalfOffset  = FRotator::ZeroRotator;
        LeftFootOffset  = FRotator::ZeroRotator;

        RightThighOffset = FRotator::ZeroRotator;
        RightCalfOffset  = FRotator::ZeroRotator;
        RightFootOffset  = FRotator::ZeroRotator;

        PelvisOffset     = FRotator::ZeroRotator;
        SpineOffset      = FRotator::ZeroRotator;
        ChestOffset      = FRotator::ZeroRotator;
        UpChestOffset    = FRotator::ZeroRotator;
        NeckOffset       = FRotator::ZeroRotator;
        HeadOffset       = FRotator::ZeroRotator;
        break;

    case ERebocapA2TPreset::Custom:
    default:
        break;
    }
}

void FAnimNode_RebocapA2T::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread);
    FAnimNode_Base::Initialize_AnyThread(Context);
    BasePose.Initialize(Context);
}

void FAnimNode_RebocapA2T::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread);
    BasePose.CacheBones(Context);

    const FBoneContainer& RequiredBones = Context.AnimInstanceProxy->GetRequiredBones();

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

    Pelvis.Initialize(RequiredBones);
    Spine.Initialize(RequiredBones);
    Chest.Initialize(RequiredBones);
    UpChest.Initialize(RequiredBones);
    Neck.Initialize(RequiredBones);
    Head.Initialize(RequiredBones);
}

void FAnimNode_RebocapA2T::Update_AnyThread(const FAnimationUpdateContext& Context)
{
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread);
    BasePose.Update(Context);
    GetEvaluateGraphExposedInputs().Execute(Context);
}

void FAnimNode_RebocapA2T::Evaluate_AnyThread(FPoseContext& Output)
{
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread);
    BasePose.Evaluate(Output);

    if (FMath::IsNearlyZero(Alpha))
    {
        return;
    }

    const FBoneContainer& BoneContainer = Output.Pose.GetBoneContainer();

    auto ApplyLocalRotation = [&](FBoneReference& BoneRef, const FRotator& OffsetRotator)
    {
        if (BoneRef.BoneIndex == INDEX_NONE || OffsetRotator.IsNearlyZero())
        {
            return;
        }

        const FCompactPoseBoneIndex CompactIndex = BoneRef.GetCompactPoseIndex(BoneContainer);
        if (CompactIndex != INDEX_NONE)
        {
            FTransform LocalTransform = Output.Pose[CompactIndex];
            
            // 局部四元数累加
            const FQuat LocalAdditiveQuat = FQuat(OffsetRotator * Alpha);
            LocalTransform.SetRotation(LocalTransform.GetRotation() * LocalAdditiveQuat);

            Output.Pose[CompactIndex] = LocalTransform;
        }
    };

    // 1. 左上肢 (Left Arm)
    ApplyLocalRotation(LeftClavicle, LeftClavicleOffset);
    ApplyLocalRotation(LeftUpperArm, LeftUpperArmOffset);
    ApplyLocalRotation(LeftLowerArm, LeftLowerArmOffset);
    ApplyLocalRotation(LeftHand, LeftHandOffset);

    // 2. 右上肢 (Right Arm)
    ApplyLocalRotation(RightClavicle, RightClavicleOffset);
    ApplyLocalRotation(RightUpperArm, RightUpperArmOffset);
    ApplyLocalRotation(RightLowerArm, RightLowerArmOffset);
    ApplyLocalRotation(RightHand, RightHandOffset);

    // 3. 左下肢 (Left Leg)
    ApplyLocalRotation(LeftThigh, LeftThighOffset);
    ApplyLocalRotation(LeftCalf, LeftCalfOffset);
    ApplyLocalRotation(LeftFoot, LeftFootOffset);

    // 4. 右下肢 (Right Leg)
    ApplyLocalRotation(RightThigh, RightThighOffset);
    ApplyLocalRotation(RightCalf, RightCalfOffset);
    ApplyLocalRotation(RightFoot, RightFootOffset);

    // 5. 躯干与头部 (Spine & Torso & Head)
    ApplyLocalRotation(Pelvis, PelvisOffset);
    ApplyLocalRotation(Spine, SpineOffset);
    ApplyLocalRotation(Chest, ChestOffset);
    ApplyLocalRotation(UpChest, UpChestOffset);
    ApplyLocalRotation(Neck, NeckOffset);
    ApplyLocalRotation(Head, HeadOffset);
}

void FAnimNode_RebocapA2T::GatherDebugData(FNodeDebugData& DebugData)
{
    const FString DebugLine = DebugData.GetNodeName(this);
    DebugData.AddDebugItem(DebugLine);
    BasePose.GatherDebugData(DebugData);
}
