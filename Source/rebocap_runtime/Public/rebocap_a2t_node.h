#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNodeBase.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "rebocap_a2t_node.generated.h"

/** A-Pose 转 T-Pose 预设模板 */
UENUM(BlueprintType)
enum class ERebocapA2TPreset : uint8
{
    UE5_Manny_Quinn UMETA(DisplayName = "Unreal Engine 5 (Manny / Quinn / MetaHuman 官方 A-Pose)"),
    MMD_Standard    UMETA(DisplayName = "MMD / 日本模型通用 A-Pose (~40°)"),
    VRoid_VRM       UMETA(DisplayName = "VRoid / VRM 常见 A-Pose (~40°)"),
    Mixamo_APose    UMETA(DisplayName = "Mixamo A-Pose 标准 (~45°)"),
    Custom          UMETA(DisplayName = "自定义微调 (Custom)")
};

/**
 * Rebocap 专属四肢与关节 A-Pose 转 T-Pose 姿态校准节点 (Local Space 局部骨骼空间)
 * 在局部骨骼空间中对四肢关节施加旋转叠加，引擎自动通过骨骼树层级向下递归传递，彻底杜绝关节焊死与脱臼撕裂。
 */
USTRUCT(BlueprintInternalUseOnly)
struct REBOCAP_RUNTIME_API FAnimNode_RebocapA2T : public FAnimNode_Base
{
    GENERATED_USTRUCT_BODY()

    FAnimNode_RebocapA2T();

    /** 输入姿态 (Local Space 局部姿态) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
    FPoseLink BasePose;

    // ==========================================
    // --- 0. 快捷预设与对称控制 ---
    // ==========================================

    /** 快捷预设选择：切换预设将自动填充下方所有四肢旋转角度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. 快捷预设与控制 (Presets & Control)", meta = (PinHiddenByDefault, DisplayName = "选择 A-Pose 预设 (Select Preset)", ToolTip = "选择预设将自动覆盖下方所有四肢旋转角度。"))
    ERebocapA2TPreset PresetTemplate;

    /** 对称修改 (默认开启)：修改左侧四肢数值时，自动镜像同步到右侧四肢 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. 快捷预设与控制 (Presets & Control)", meta = (PinHiddenByDefault, DisplayName = "对称修改 (Symmetrical Edit)", ToolTip = "开启后，修改左侧肢体（锁骨/大臂/小臂/手/大腿/小腿/脚）的旋转角度时，自动镜像更新对应的右侧肢体。"))
    bool bMirrorEdit;

    /** 对称修改 X轴 (Roll) 符号反转：勾选 = 负正对称 (-X)，未勾选 = 正正同向对称 (+X) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. 快捷预设与控制 (Presets & Control)", meta = (EditCondition = "bMirrorEdit", PinHiddenByDefault, DisplayName = "X轴对称反转 (-X / +X)", ToolTip = "勾选后右侧 X (Roll) 符号反转 (-X，负正对称)；未勾选则右侧 X 与左侧保持相同符号 (+X，正正对称)。"))
    bool bMirrorInvertRoll;

    /** 对称修改 Y轴 (Pitch) 符号反转：勾选 = 负正对称 (-Y)，未勾选 = 正正同向对称 (+Y) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. 快捷预设与控制 (Presets & Control)", meta = (EditCondition = "bMirrorEdit", PinHiddenByDefault, DisplayName = "Y轴对称反转 (-Y / +Y)", ToolTip = "勾选后右侧 Y (Pitch) 符号反转 (-Y)；未勾选则右侧 Y 与左侧保持相同符号 (+Y)。"))
    bool bMirrorInvertPitch;

    /** 对称修改 Z轴 (Yaw) 符号反转：勾选 = 负正对称 (-Z)，未勾选 = 正正同向对称 (+Z) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. 快捷预设与控制 (Presets & Control)", meta = (EditCondition = "bMirrorEdit", PinHiddenByDefault, DisplayName = "Z轴对称反转 (-Z / +Z)", ToolTip = "勾选后右侧 Z (Yaw) 符号反转 (-Z)；未勾选则右侧 Z 与左侧保持相同符号 (+Z)。"))
    bool bMirrorInvertYaw;

    /** 应用预设角度 */
    void ApplyPreset(ERebocapA2TPreset InPreset);

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

    /** 左大臂旋转偏移（默认 Pitch 40° 抬手） */
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

    /** 右大臂旋转偏移（默认 Pitch 40° 抬手） */
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

    /** 左大腿旋转偏移 */
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

    /** 右大腿旋转偏移 */
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

    /** 校准生效强度 (0.0 = 不改变原始姿态, 1.0 = 100% 展开为 T-Pose) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (PinShownByDefault, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Alpha (校准强度权重)"))
    float Alpha;

    // --- 虚函数重写 ---
    virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
    virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
    virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
    virtual void Evaluate_AnyThread(FPoseContext& Output) override;
    virtual void GatherDebugData(FNodeDebugData& DebugData) override;
};
