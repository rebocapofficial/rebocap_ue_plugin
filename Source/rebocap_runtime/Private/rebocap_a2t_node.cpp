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

FString FAnimNode_RebocapA2T::ToJsonString() const
{
    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
    RootObject->SetStringField(TEXT("version"), TEXT("2.0"));
    RootObject->SetStringField(TEXT("type"), TEXT("rebocap_a2t_calibration"));
    RootObject->SetIntegerField(TEXT("preset_template"), static_cast<int32>(PresetTemplate));

    // 对称设置
    TSharedPtr<FJsonObject> MirrorObject = MakeShared<FJsonObject>();
    MirrorObject->SetBoolField(TEXT("mirror_edit"), bMirrorEdit);
    MirrorObject->SetBoolField(TEXT("invert_roll"), bMirrorInvertRoll);
    MirrorObject->SetBoolField(TEXT("invert_pitch"), bMirrorInvertPitch);
    MirrorObject->SetBoolField(TEXT("invert_yaw"), bMirrorInvertYaw);
    RootObject->SetObjectField(TEXT("mirror_settings"), MirrorObject);

    // 旋转偏移
    TSharedPtr<FJsonObject> RotObject = MakeShared<FJsonObject>();
    auto WriteRot = [&](const TCHAR* Key, const FRotator& Rot)
    {
        TArray<TSharedPtr<FJsonValue>> Arr;
        Arr.Add(MakeShared<FJsonValueNumber>(Rot.Pitch));
        Arr.Add(MakeShared<FJsonValueNumber>(Rot.Yaw));
        Arr.Add(MakeShared<FJsonValueNumber>(Rot.Roll));
        RotObject->SetArrayField(Key, Arr);
    };

    WriteRot(TEXT("left_clavicle"), LeftClavicleOffset);
    WriteRot(TEXT("left_upperarm"), LeftUpperArmOffset);
    WriteRot(TEXT("left_lowerarm"), LeftLowerArmOffset);
    WriteRot(TEXT("left_hand"), LeftHandOffset);

    WriteRot(TEXT("right_clavicle"), RightClavicleOffset);
    WriteRot(TEXT("right_upperarm"), RightUpperArmOffset);
    WriteRot(TEXT("right_lowerarm"), RightLowerArmOffset);
    WriteRot(TEXT("right_hand"), RightHandOffset);

    WriteRot(TEXT("left_thigh"), LeftThighOffset);
    WriteRot(TEXT("left_calf"), LeftCalfOffset);
    WriteRot(TEXT("left_foot"), LeftFootOffset);

    WriteRot(TEXT("right_thigh"), RightThighOffset);
    WriteRot(TEXT("right_calf"), RightCalfOffset);
    WriteRot(TEXT("right_foot"), RightFootOffset);

    WriteRot(TEXT("pelvis"), PelvisOffset);
    WriteRot(TEXT("spine"), SpineOffset);
    WriteRot(TEXT("chest"), ChestOffset);
    WriteRot(TEXT("up_chest"), UpChestOffset);
    WriteRot(TEXT("neck"), NeckOffset);
    WriteRot(TEXT("head"), HeadOffset);
    RootObject->SetObjectField(TEXT("bone_rotations"), RotObject);

    // 骨骼绑定名称
    TSharedPtr<FJsonObject> BonesObject = MakeShared<FJsonObject>();
    BonesObject->SetStringField(TEXT("left_clavicle"), LeftClavicle.BoneName.ToString());
    BonesObject->SetStringField(TEXT("left_upperarm"), LeftUpperArm.BoneName.ToString());
    BonesObject->SetStringField(TEXT("left_lowerarm"), LeftLowerArm.BoneName.ToString());
    BonesObject->SetStringField(TEXT("left_hand"), LeftHand.BoneName.ToString());

    BonesObject->SetStringField(TEXT("right_clavicle"), RightClavicle.BoneName.ToString());
    BonesObject->SetStringField(TEXT("right_upperarm"), RightUpperArm.BoneName.ToString());
    BonesObject->SetStringField(TEXT("right_lowerarm"), RightLowerArm.BoneName.ToString());
    BonesObject->SetStringField(TEXT("right_hand"), RightHand.BoneName.ToString());

    BonesObject->SetStringField(TEXT("left_thigh"), LeftThigh.BoneName.ToString());
    BonesObject->SetStringField(TEXT("left_calf"), LeftCalf.BoneName.ToString());
    BonesObject->SetStringField(TEXT("left_foot"), LeftFoot.BoneName.ToString());

    BonesObject->SetStringField(TEXT("right_thigh"), RightThigh.BoneName.ToString());
    BonesObject->SetStringField(TEXT("right_calf"), RightCalf.BoneName.ToString());
    BonesObject->SetStringField(TEXT("right_foot"), RightFoot.BoneName.ToString());

    BonesObject->SetStringField(TEXT("pelvis"), Pelvis.BoneName.ToString());
    BonesObject->SetStringField(TEXT("spine"), Spine.BoneName.ToString());
    BonesObject->SetStringField(TEXT("chest"), Chest.BoneName.ToString());
    BonesObject->SetStringField(TEXT("up_chest"), UpChest.BoneName.ToString());
    BonesObject->SetStringField(TEXT("neck"), Neck.BoneName.ToString());
    BonesObject->SetStringField(TEXT("head"), Head.BoneName.ToString());
    RootObject->SetObjectField(TEXT("bone_names"), BonesObject);

    RootObject->SetNumberField(TEXT("alpha"), Alpha);

    FString OutputString;
    TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);

    return OutputString;
}

bool FAnimNode_RebocapA2T::FromJsonString(const FString& InJsonString)
{
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InJsonString);
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        return false;
    }

    PresetTemplate = ERebocapA2TPreset::Custom;

    // 1. 对称设置
    if (RootObject->HasField(TEXT("mirror_settings")))
    {
        TSharedPtr<FJsonObject> MirrorObject = RootObject->GetObjectField(TEXT("mirror_settings"));
        if (MirrorObject.IsValid())
        {
            if (MirrorObject->HasField(TEXT("mirror_edit"))) bMirrorEdit = MirrorObject->GetBoolField(TEXT("mirror_edit"));
            if (MirrorObject->HasField(TEXT("invert_roll"))) bMirrorInvertRoll = MirrorObject->GetBoolField(TEXT("invert_roll"));
            if (MirrorObject->HasField(TEXT("invert_pitch"))) bMirrorInvertPitch = MirrorObject->GetBoolField(TEXT("invert_pitch"));
            if (MirrorObject->HasField(TEXT("invert_yaw"))) bMirrorInvertYaw = MirrorObject->GetBoolField(TEXT("invert_yaw"));
        }
    }

    // 2. 旋转偏移
    if (RootObject->HasField(TEXT("bone_rotations")))
    {
        TSharedPtr<FJsonObject> RotObject = RootObject->GetObjectField(TEXT("bone_rotations"));
        if (RotObject.IsValid())
        {
            auto ReadRot = [&](const TCHAR* Key, FRotator& OutRot)
            {
                if (RotObject->HasField(Key))
                {
                    TArray<TSharedPtr<FJsonValue>> Arr = RotObject->GetArrayField(Key);
                    if (Arr.Num() >= 3)
                    {
                        OutRot.Pitch = Arr[0]->AsNumber();
                        OutRot.Yaw = Arr[1]->AsNumber();
                        OutRot.Roll = Arr[2]->AsNumber();
                    }
                }
            };

            ReadRot(TEXT("left_clavicle"), LeftClavicleOffset);
            ReadRot(TEXT("left_upperarm"), LeftUpperArmOffset);
            ReadRot(TEXT("left_lowerarm"), LeftLowerArmOffset);
            ReadRot(TEXT("left_hand"), LeftHandOffset);

            ReadRot(TEXT("right_clavicle"), RightClavicleOffset);
            ReadRot(TEXT("right_upperarm"), RightUpperArmOffset);
            ReadRot(TEXT("right_lowerarm"), RightLowerArmOffset);
            ReadRot(TEXT("right_hand"), RightHandOffset);

            ReadRot(TEXT("left_thigh"), LeftThighOffset);
            ReadRot(TEXT("left_calf"), LeftCalfOffset);
            ReadRot(TEXT("left_foot"), LeftFootOffset);

            ReadRot(TEXT("right_thigh"), RightThighOffset);
            ReadRot(TEXT("right_calf"), RightCalfOffset);
            ReadRot(TEXT("right_foot"), RightFootOffset);

            ReadRot(TEXT("pelvis"), PelvisOffset);
            ReadRot(TEXT("spine"), SpineOffset);
            ReadRot(TEXT("chest"), ChestOffset);
            ReadRot(TEXT("up_chest"), UpChestOffset);
            ReadRot(TEXT("neck"), NeckOffset);
            ReadRot(TEXT("head"), HeadOffset);
        }
    }

    // 3. 骨骼绑定名称
    if (RootObject->HasField(TEXT("bone_names")))
    {
        TSharedPtr<FJsonObject> BonesObject = RootObject->GetObjectField(TEXT("bone_names"));
        if (BonesObject.IsValid())
        {
            auto ReadBone = [&](const TCHAR* Key, FBoneReference& BoneRef)
            {
                if (BonesObject->HasField(Key))
                {
                    const FString BoneNameStr = BonesObject->GetStringField(Key);
                    if (!BoneNameStr.IsEmpty())
                    {
                        BoneRef.BoneName = FName(*BoneNameStr);
                    }
                }
            };

            ReadBone(TEXT("left_clavicle"), LeftClavicle);
            ReadBone(TEXT("left_upperarm"), LeftUpperArm);
            ReadBone(TEXT("left_lowerarm"), LeftLowerArm);
            ReadBone(TEXT("left_hand"), LeftHand);

            ReadBone(TEXT("right_clavicle"), RightClavicle);
            ReadBone(TEXT("right_upperarm"), RightUpperArm);
            ReadBone(TEXT("right_lowerarm"), RightLowerArm);
            ReadBone(TEXT("right_hand"), RightHand);

            ReadBone(TEXT("left_thigh"), LeftThigh);
            ReadBone(TEXT("left_calf"), LeftCalf);
            ReadBone(TEXT("left_foot"), LeftFoot);

            ReadBone(TEXT("right_thigh"), RightThigh);
            ReadBone(TEXT("right_calf"), RightCalf);
            ReadBone(TEXT("right_foot"), RightFoot);

            ReadBone(TEXT("pelvis"), Pelvis);
            ReadBone(TEXT("spine"), Spine);
            ReadBone(TEXT("chest"), Chest);
            ReadBone(TEXT("up_chest"), UpChest);
            ReadBone(TEXT("neck"), Neck);
            ReadBone(TEXT("head"), Head);
        }
    }

    if (RootObject->HasField(TEXT("alpha")))
    {
        Alpha = RootObject->GetNumberField(TEXT("alpha"));
    }

    return true;
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
