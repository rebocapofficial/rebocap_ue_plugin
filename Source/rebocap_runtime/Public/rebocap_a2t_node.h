#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "rebocap_a2t_node.generated.h"

/** A-Pose to T-Pose Presets */
UENUM(BlueprintType)
enum class ERebocapA2TPreset : uint8
{
    UE5_Manny_Quinn UMETA(DisplayName = "Unreal Engine 5 (Manny / Quinn / MetaHuman Standard A-Pose)"),
    MMD_Standard    UMETA(DisplayName = "MMD / Japanese Models Standard A-Pose (~40°)"),
    VRoid_VRM       UMETA(DisplayName = "VRoid / VRM Common A-Pose (~40°)"),
    Mixamo_APose    UMETA(DisplayName = "Mixamo Standard A-Pose (~45°)"),
    Custom          UMETA(DisplayName = "Custom")
};

/**
 * Rebocap A-Pose to T-Pose Limb Calibrator Node (Local Space)
 * In Local Space, rotates limbs and lets the engine hierarchical FK chain propagate smoothly to all child and twist joints.
 */
USTRUCT(BlueprintInternalUseOnly)
struct REBOCAP_RUNTIME_API FAnimNode_RebocapA2T : public FAnimNode_Base
{
    GENERATED_USTRUCT_BODY()

    FAnimNode_RebocapA2T();

    /** Base Pose (Local Space) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
    FPoseLink BasePose;

    // ==========================================
    // --- 0. Presets & Control ---
    // ==========================================

    /** Preset Template */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Control", meta = (PinHiddenByDefault, DisplayName = "Preset Template", ToolTip = "快捷预设模板。 / Preset Template."))
    ERebocapA2TPreset PresetTemplate;

    /** Symmetrical Edit (Mirror left changes to right) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Control", meta = (PinHiddenByDefault, DisplayName = "Symmetrical Edit", ToolTip = "对称修改开关（默认开启）。 / Symmetrical Edit (Default ON)."))
    bool bMirrorEdit;

    /** Mirror Invert Roll (X) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Control", meta = (EditCondition = "bMirrorEdit", PinHiddenByDefault, DisplayName = "Invert Roll (-X / +X)", ToolTip = "X轴对称反转 (默认关闭)。 / Invert Roll (-X / +X)."))
    bool bMirrorInvertRoll;

    /** Mirror Invert Pitch (Y) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Control", meta = (EditCondition = "bMirrorEdit", PinHiddenByDefault, DisplayName = "Invert Pitch (-Y / +Y)", ToolTip = "Y轴对称反转 (默认关闭)。 / Invert Pitch (-Y / +Y)."))
    bool bMirrorInvertPitch;

    /** Mirror Invert Yaw (Z) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Control", meta = (EditCondition = "bMirrorEdit", PinHiddenByDefault, DisplayName = "Invert Yaw (-Z / +Z)", ToolTip = "Z轴对称反转 (默认关闭)。 / Invert Yaw (-Z / +Z)."))
    bool bMirrorInvertYaw;

    /** Apply preset angles */
    void ApplyPreset(ERebocapA2TPreset InPreset);

    // ==========================================
    // --- 1. Left Arm ---
    // ==========================================

    /** Left Clavicle Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left Clavicle Bone"))
    FBoneReference LeftClavicle;

    /** Left Clavicle Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left Clavicle Offset", ToolTip = "Left clavicle rotation offset in local space. / 左锁骨局部旋转偏移。"))
    FRotator LeftClavicleOffset;

    /** Left UpperArm Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left UpperArm Bone"))
    FBoneReference LeftUpperArm;

    /** Left UpperArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left UpperArm Offset", ToolTip = "Left upperarm rotation offset in local space. / 左大臂局部旋转偏移。"))
    FRotator LeftUpperArmOffset;

    /** Left LowerArm Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left LowerArm Bone"))
    FBoneReference LeftLowerArm;

    /** Left LowerArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left LowerArm Offset", ToolTip = "Left lowerarm rotation offset in local space. / 左小臂局部旋转偏移。"))
    FRotator LeftLowerArmOffset;

    /** Left Hand Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left Hand Bone"))
    FBoneReference LeftHand;

    /** Left Hand Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left Hand Offset", ToolTip = "Left hand rotation offset in local space. / 左手局部旋转偏移。"))
    FRotator LeftHandOffset;

    // ==========================================
    // --- 2. Right Arm ---
    // ==========================================

    /** Right Clavicle Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right Clavicle Bone"))
    FBoneReference RightClavicle;

    /** Right Clavicle Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (PinHiddenByDefault, DisplayName = "Right Clavicle Offset", ToolTip = "Right clavicle rotation offset in local space. / 右锁骨局部旋转偏移。"))
    FRotator RightClavicleOffset;

    /** Right UpperArm Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right UpperArm Bone"))
    FBoneReference RightUpperArm;

    /** Right UpperArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (PinHiddenByDefault, DisplayName = "Right UpperArm Offset", ToolTip = "Right upperarm rotation offset in local space. / 右大臂局部旋转偏移。"))
    FRotator RightUpperArmOffset;

    /** Right LowerArm Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right LowerArm Bone"))
    FBoneReference RightLowerArm;

    /** Right LowerArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (PinHiddenByDefault, DisplayName = "Right LowerArm Offset", ToolTip = "Right lowerarm rotation offset in local space. / 右小臂局部旋转偏移。"))
    FRotator RightLowerArmOffset;

    /** Right Hand Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right Hand Bone"))
    FBoneReference RightHand;

    /** Right Hand Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (PinHiddenByDefault, DisplayName = "Right Hand Offset", ToolTip = "Right hand rotation offset in local space. / 右手局部旋转偏移。"))
    FRotator RightHandOffset;

    // ==========================================
    // --- 3. Left Leg ---
    // ==========================================

    /** Left Thigh Bone */
    UPROPERTY(EditAnywhere, Category = "3. Left Leg", meta = (DisplayName = "Left Thigh Bone"))
    FBoneReference LeftThigh;

    /** Left Thigh Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Left Leg", meta = (PinHiddenByDefault, DisplayName = "Left Thigh Offset", ToolTip = "Left thigh rotation offset in local space. / 左大腿局部旋转偏移。"))
    FRotator LeftThighOffset;

    /** Left Calf Bone */
    UPROPERTY(EditAnywhere, Category = "3. Left Leg", meta = (DisplayName = "Left Calf Bone"))
    FBoneReference LeftCalf;

    /** Left Calf Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Left Leg", meta = (PinHiddenByDefault, DisplayName = "Left Calf Offset", ToolTip = "Left calf rotation offset in local space. / 左小腿局部旋转偏移。"))
    FRotator LeftCalfOffset;

    /** Left Foot Bone */
    UPROPERTY(EditAnywhere, Category = "3. Left Leg", meta = (DisplayName = "Left Foot Bone"))
    FBoneReference LeftFoot;

    /** Left Foot Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Left Leg", meta = (PinHiddenByDefault, DisplayName = "Left Foot Offset", ToolTip = "Left foot rotation offset in local space. / 左脚踝局部旋转偏移。"))
    FRotator LeftFootOffset;

    // ==========================================
    // --- 4. Right Leg ---
    // ==========================================

    /** Right Thigh Bone */
    UPROPERTY(EditAnywhere, Category = "4. Right Leg", meta = (DisplayName = "Right Thigh Bone"))
    FBoneReference RightThigh;

    /** Right Thigh Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Right Leg", meta = (PinHiddenByDefault, DisplayName = "Right Thigh Offset", ToolTip = "Right thigh rotation offset in local space. / 右大腿局部旋转偏移。"))
    FRotator RightThighOffset;

    /** Right Calf Bone */
    UPROPERTY(EditAnywhere, Category = "4. Right Leg", meta = (DisplayName = "Right Calf Bone"))
    FBoneReference RightCalf;

    /** Right Calf Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Right Leg", meta = (PinHiddenByDefault, DisplayName = "Right Calf Offset", ToolTip = "Right calf rotation offset in local space. / 右小腿局部旋转偏移。"))
    FRotator RightCalfOffset;

    /** Right Foot Bone */
    UPROPERTY(EditAnywhere, Category = "4. Right Leg", meta = (DisplayName = "Right Foot Bone"))
    FBoneReference RightFoot;

    /** Right Foot Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Right Leg", meta = (PinHiddenByDefault, DisplayName = "Right Foot Offset", ToolTip = "Right foot rotation offset in local space. / 右脚踝局部旋转偏移。"))
    FRotator RightFootOffset;

    // ==========================================
    // --- 5. Spine & Torso & Head ---
    // ==========================================

    /** Pelvis Bone */
    UPROPERTY(EditAnywhere, Category = "5. Spine & Torso & Head", meta = (DisplayName = "Pelvis Bone"))
    FBoneReference Pelvis;

    /** Pelvis Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Spine & Torso & Head", meta = (PinHiddenByDefault, DisplayName = "Pelvis Offset", ToolTip = "Pelvis rotation offset in local space. / 骨盆/根骨骼局部旋转偏移。"))
    FRotator PelvisOffset;

    /** Spine / Waist Bone */
    UPROPERTY(EditAnywhere, Category = "5. Spine & Torso & Head", meta = (DisplayName = "Spine / Waist Bone"))
    FBoneReference Spine;

    /** Spine / Waist Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Spine & Torso & Head", meta = (PinHiddenByDefault, DisplayName = "Spine / Waist Offset", ToolTip = "Spine/waist rotation offset in local space. / 腰椎/脊柱局部旋转偏移。"))
    FRotator SpineOffset;

    /** Chest Bone */
    UPROPERTY(EditAnywhere, Category = "5. Spine & Torso & Head", meta = (DisplayName = "Chest Bone"))
    FBoneReference Chest;

    /** Chest Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Spine & Torso & Head", meta = (PinHiddenByDefault, DisplayName = "Chest Offset", ToolTip = "Chest rotation offset in local space. / 胸腔局部旋转偏移。"))
    FRotator ChestOffset;

    /** Up Chest Bone */
    UPROPERTY(EditAnywhere, Category = "5. Spine & Torso & Head", meta = (DisplayName = "Up Chest Bone"))
    FBoneReference UpChest;

    /** Up Chest Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Spine & Torso & Head", meta = (PinHiddenByDefault, DisplayName = "Up Chest Offset", ToolTip = "Up chest rotation offset in local space. / 上胸局部旋转偏移。"))
    FRotator UpChestOffset;

    /** Neck Bone */
    UPROPERTY(EditAnywhere, Category = "5. Spine & Torso & Head", meta = (DisplayName = "Neck Bone"))
    FBoneReference Neck;

    /** Neck Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Spine & Torso & Head", meta = (PinHiddenByDefault, DisplayName = "Neck Offset", ToolTip = "Neck rotation offset in local space. / 颈部/脖子局部旋转偏移。"))
    FRotator NeckOffset;

    /** Head Bone */
    UPROPERTY(EditAnywhere, Category = "5. Spine & Torso & Head", meta = (DisplayName = "Head Bone"))
    FBoneReference Head;

    /** Head Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Spine & Torso & Head", meta = (PinHiddenByDefault, DisplayName = "Head Offset", ToolTip = "Head rotation offset in local space. / 头部局部旋转偏移。"))
    FRotator HeadOffset;

    /** Alpha Weight */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (PinShownByDefault, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Alpha", ToolTip = "A2T 姿态校准强度权重 / Alpha."))
    float Alpha;

    // --- 虚函数重写 ---
    virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
    virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
    virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
    virtual void Evaluate_AnyThread(FPoseContext& Output) override;
    virtual void GatherDebugData(FNodeDebugData& DebugData) override;
};
