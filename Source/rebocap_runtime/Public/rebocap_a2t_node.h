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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Control", meta = (PinHiddenByDefault, DisplayName = "Select Preset Template", ToolTip = "快捷预设模板。 / Preset Template."))
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

    /** Calculate mirrored rotator with invert flags */
    FRotator CalculateMirroredRotator(const FRotator& InRot) const;

    /** Synchronize left limb rotation changes to right limb when mirror edit is enabled */
    void SyncMirrorLimbOffsets(FName ChangedPropertyName = NAME_None);

    /** Export A2T calibration to JSON string */
    FString ToJsonString() const;

    /** Import A2T calibration from JSON string */
    bool FromJsonString(const FString& InJsonString);

    // ==========================================
    // --- 1. Left Arm ---
    // ==========================================

    /** Left Collar / Clavicle Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left Collar / Clavicle Bone"))
    FBoneReference LeftClavicle;

    /** Left Collar / Clavicle Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left Collar / Clavicle Offset", ToolTip = "Left collar / clavicle rotation offset in local space. / 左锁骨局部旋转偏移。"))
    FRotator LeftClavicleOffset;

    /** Left Shoulder / UpperArm Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left Shoulder / UpperArm Bone"))
    FBoneReference LeftUpperArm;

    /** Left Shoulder / UpperArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left Shoulder / UpperArm Offset", ToolTip = "Left shoulder / upperarm rotation offset in local space. / 左大臂局部旋转偏移。"))
    FRotator LeftUpperArmOffset;

    /** Left Elbow / LowerArm Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left Elbow / LowerArm Bone"))
    FBoneReference LeftLowerArm;

    /** Left Elbow / LowerArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left Elbow / LowerArm Offset", ToolTip = "Left elbow / lowerarm rotation offset in local space. / 左小臂局部旋转偏移。"))
    FRotator LeftLowerArmOffset;

    /** Left Wrist / Hand Bone */
    UPROPERTY(EditAnywhere, Category = "1. Left Arm", meta = (DisplayName = "Left Wrist / Hand Bone"))
    FBoneReference LeftHand;

    /** Left Wrist / Hand Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Left Arm", meta = (PinHiddenByDefault, DisplayName = "Left Wrist / Hand Offset", ToolTip = "Left wrist / hand rotation offset in local space. / 左手局部旋转偏移。"))
    FRotator LeftHandOffset;

    // ==========================================
    // --- 2. Right Arm ---
    // ==========================================

    /** Right Collar / Clavicle Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right Collar / Clavicle Bone"))
    FBoneReference RightClavicle;

    /** Right Collar / Clavicle Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Collar / Clavicle Offset", ToolTip = "Right collar / clavicle rotation offset in local space. / 右锁骨局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightClavicleOffset;

    /** Right Shoulder / UpperArm Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right Shoulder / UpperArm Bone"))
    FBoneReference RightUpperArm;

    /** Right Shoulder / UpperArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Shoulder / UpperArm Offset", ToolTip = "Right shoulder / upperarm rotation offset in local space. / 右大臂局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightUpperArmOffset;

    /** Right Elbow / LowerArm Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right Elbow / LowerArm Bone"))
    FBoneReference RightLowerArm;

    /** Right Elbow / LowerArm Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Elbow / LowerArm Offset", ToolTip = "Right elbow / lowerarm rotation offset in local space. / 右小臂局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightLowerArmOffset;

    /** Right Wrist / Hand Bone */
    UPROPERTY(EditAnywhere, Category = "2. Right Arm", meta = (DisplayName = "Right Wrist / Hand Bone"))
    FBoneReference RightHand;

    /** Right Wrist / Hand Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Right Arm", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Wrist / Hand Offset", ToolTip = "Right wrist / hand rotation offset in local space. / 右手局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightHandOffset;

    // ==========================================
    // --- 3. Left Leg ---
    // ==========================================

    /** Left Hip / Thigh Bone */
    UPROPERTY(EditAnywhere, Category = "3. Left Leg", meta = (DisplayName = "Left Hip / Thigh Bone"))
    FBoneReference LeftThigh;

    /** Left Hip / Thigh Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Left Leg", meta = (PinHiddenByDefault, DisplayName = "Left Hip / Thigh Offset", ToolTip = "Left hip / thigh rotation offset in local space. / 左大腿局部旋转偏移。"))
    FRotator LeftThighOffset;

    /** Left Knee / Calf Bone */
    UPROPERTY(EditAnywhere, Category = "3. Left Leg", meta = (DisplayName = "Left Knee / Calf Bone"))
    FBoneReference LeftCalf;

    /** Left Knee / Calf Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Left Leg", meta = (PinHiddenByDefault, DisplayName = "Left Knee / Calf Offset", ToolTip = "Left knee / calf rotation offset in local space. / 左小腿局部旋转偏移。"))
    FRotator LeftCalfOffset;

    /** Left Ankle / Foot Bone */
    UPROPERTY(EditAnywhere, Category = "3. Left Leg", meta = (DisplayName = "Left Ankle / Foot Bone"))
    FBoneReference LeftFoot;

    /** Left Ankle / Foot Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Left Leg", meta = (PinHiddenByDefault, DisplayName = "Left Ankle / Foot Offset", ToolTip = "Left ankle / foot rotation offset in local space. / 左脚踝局部旋转偏移。"))
    FRotator LeftFootOffset;

    // ==========================================
    // --- 4. Right Leg ---
    // ==========================================

    /** Right Hip / Thigh Bone */
    UPROPERTY(EditAnywhere, Category = "4. Right Leg", meta = (DisplayName = "Right Hip / Thigh Bone"))
    FBoneReference RightThigh;

    /** Right Hip / Thigh Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Right Leg", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Hip / Thigh Offset", ToolTip = "Right thigh rotation offset in local space. / 右大腿局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightThighOffset;

    /** Right Knee / Calf Bone */
    UPROPERTY(EditAnywhere, Category = "4. Right Leg", meta = (DisplayName = "Right Knee / Calf Bone"))
    FBoneReference RightCalf;

    /** Right Knee / Calf Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Right Leg", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Knee / Calf Offset", ToolTip = "Right knee / calf rotation offset in local space. / 右小腿局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightCalfOffset;

    /** Right Ankle / Foot Bone */
    UPROPERTY(EditAnywhere, Category = "4. Right Leg", meta = (DisplayName = "Right Ankle / Foot Bone"))
    FBoneReference RightFoot;

    /** Right Ankle / Foot Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Right Leg", meta = (EditCondition = "!bMirrorEdit", PinHiddenByDefault, DisplayName = "Right Ankle / Foot Offset", ToolTip = "Right ankle / foot rotation offset in local space. / 右脚踝局部旋转偏移（开启镜像编辑时由左侧同步，关闭镜像后可单独手改）。"))
    FRotator RightFootOffset;

    // ==========================================
    // --- 5. Root & Spine & Head ---
    // ==========================================

    /** Pelvis / Hips Bone */
    UPROPERTY(EditAnywhere, Category = "5. Root & Spine & Head", meta = (DisplayName = "Pelvis / Hips Bone"))
    FBoneReference Pelvis;

    /** Pelvis / Hips Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Root & Spine & Head", meta = (PinHiddenByDefault, DisplayName = "Pelvis / Hips Offset", ToolTip = "Pelvis / hips rotation offset in local space. / 骨盆/根骨骼局部旋转偏移。"))
    FRotator PelvisOffset;

    /** Spine1 / Waist Bone */
    UPROPERTY(EditAnywhere, Category = "5. Root & Spine & Head", meta = (DisplayName = "Spine1 / Waist Bone"))
    FBoneReference Spine;

    /** Spine1 / Waist Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Root & Spine & Head", meta = (PinHiddenByDefault, DisplayName = "Spine1 / Waist Offset", ToolTip = "Spine1 / waist rotation offset in local space. / 腰椎/脊柱局部旋转偏移。"))
    FRotator SpineOffset;

    /** Spine2 / Chest Bone */
    UPROPERTY(EditAnywhere, Category = "5. Root & Spine & Head", meta = (DisplayName = "Spine2 / Chest Bone"))
    FBoneReference Chest;

    /** Spine2 / Chest Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Root & Spine & Head", meta = (PinHiddenByDefault, DisplayName = "Spine2 / Chest Offset", ToolTip = "Spine2 / chest rotation offset in local space. / 胸腔局部旋转偏移。"))
    FRotator ChestOffset;

    /** Spine3 / Up Chest Bone */
    UPROPERTY(EditAnywhere, Category = "5. Root & Spine & Head", meta = (DisplayName = "Spine3 / Up Chest Bone"))
    FBoneReference UpChest;

    /** Spine3 / Up Chest Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Root & Spine & Head", meta = (PinHiddenByDefault, DisplayName = "Spine3 / Up Chest Offset", ToolTip = "Spine3 / up chest rotation offset in local space. / 上胸局部旋转偏移。"))
    FRotator UpChestOffset;

    /** Neck Bone */
    UPROPERTY(EditAnywhere, Category = "5. Root & Spine & Head", meta = (DisplayName = "Neck Bone"))
    FBoneReference Neck;

    /** Neck Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Root & Spine & Head", meta = (PinHiddenByDefault, DisplayName = "Neck Offset", ToolTip = "Neck rotation offset in local space. / 颈部/脖子局部旋转偏移。"))
    FRotator NeckOffset;

    /** Head Bone */
    UPROPERTY(EditAnywhere, Category = "5. Root & Spine & Head", meta = (DisplayName = "Head Bone"))
    FBoneReference Head;

    /** Head Rotation Offset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "5. Root & Spine & Head", meta = (PinHiddenByDefault, DisplayName = "Head Offset", ToolTip = "Head rotation offset in local space. / 头部局部旋转偏移。"))
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
