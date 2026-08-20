#pragma once

#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"
#include "rebocap_body_remap_asset.generated.h"

/** 常用骨骼命名预设规范 */
UENUM(BlueprintType)
enum class ERebocapBonePreset : uint8 {
  UE5_Mannequin UMETA(DisplayName = "UE5 Manny / Quinn (官方标准骨骼)"),
  UE4_Mannequin UMETA(DisplayName = "UE4 SK_Mannequin (经典小白人骨骼)"),
  Mixamo UMETA(DisplayName = "Adobe Mixamo (无前缀: Hips, Spine...)"),
  Mixamo_Prefix UMETA(DisplayName = "Adobe Mixamo (带前缀: mixamorig:Hips...)"),
  MMD_Japanese UMETA(DisplayName = "MMD 经典日文骨骼 (センター, 上半身, 左腕...)"),
  VRM_Humanoid UMETA(DisplayName = "VRM / VRoid / Unity Humanoid 骨骼规范"),
  Rebocap_Standard UMETA(DisplayName = "Rebocap 24骨标准命名 (默认值)"),
  Custom UMETA(DisplayName = "Custom (自定义模式)")
};

/** 
 * Rebocap 骨骼映射基础资产
 * 用于将标准 Rebocap 24 骨节点映射到当前角色的对应骨骼名称。
 * 支持顶部下拉切换预设一键自动填词，并允许随时手动微调。
 */
UCLASS(BlueprintType, Blueprintable, meta = (ToolTip = "Rebocap 骨骼映射资产，用于将标准 Rebocap 24 骨节点映射到当前角色的对应骨骼名称。\n支持在面板顶部选择预设一键自动填词。"))
class REBOCAP_RUNTIME_API URebocapMapData : public ULiveLinkRemapAsset {
  GENERATED_BODY()

 public:
  URebocapMapData();

  FName GetRemappedBoneName_Implementation(FName CurveName) const final;

  void Initialize() final { InitializeTMap(); }

  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void InitializeTMap();

  /** 
   * 快捷预设选择：切换预设将自动将下方所有输入框覆盖为对应的骨骼名称。
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. 快捷骨骼预设 (Preset Template)", meta = (DisplayName = "选择骨骼预设 (Select Preset)", ToolTip = "选择预设将自动将下方所有输入框覆盖为对应的骨骼名称。填充后可继续手动微调。"))
  ERebocapBonePreset PresetTemplate = ERebocapBonePreset::UE5_Mannequin;

  /** 
   * 点击立即将当前所选预设的骨骼名称批量填充到下方输入框中。
   */
  UFUNCTION(BlueprintCallable, CallInEditor, Category = "0. 快捷骨骼预设 (Preset Template)", meta = (DisplayName = "一键填充当前预设 (Apply Preset)", ToolTip = "点击后将当前所选预设的骨骼名称批量填充到下方输入框中。"))
  void ApplyPreset(ERebocapBonePreset InPreset);

#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "0. 映射表预览 (Mapping Preview)")
  TMap<FName, FName> name_mapping_;

  // --- 1. 躯干与头部 (Root & Spine & Head) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Pelvis / Hips (盆骨/根骨骼)", ToolTip = "标准动捕根骨骼。\n· 对应 UE5: pelvis\n· 对应 Mixamo: Hips\n· 对应 VRM: Hips"))
  FName rebocap_pelvis_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Spine1 / Spine_01 (脊椎1/下脊椎)", ToolTip = "下部脊椎骨骼。\n· 对应 UE5: spine_01\n· 对应 Mixamo: Spine\n· 对应 VRM: Spine"))
  FName spine1_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Spine2 / Spine_02 (脊椎2/中脊椎)", ToolTip = "中部脊椎骨骼。\n· 对应 UE5: spine_02\n· 对应 Mixamo: Spine1\n· 对应 VRM: Spine"))
  FName spine2_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Spine3 / Spine_03 (脊椎3/胸腔)", ToolTip = "上部脊椎/胸腔骨骼。\n· 对应 UE5: spine_03\n· 对应 Mixamo: Spine2 / Chest\n· 对应 VRM: Chest"))
  FName spine3_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Neck / Neck_01 (脖子/颈部)", ToolTip = "颈部骨骼。\n· 对应 UE5: neck_01\n· 对应 Mixamo: Neck\n· 对应 VRM: Neck"))
  FName neck_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Head (头部)", ToolTip = "头部骨骼。\n· 对应 UE5: head\n· 对应 Mixamo: Head\n· 对应 VRM: Head"))
  FName head_;

  // --- 2. 左上肢 (Left Arm) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Collar / Clavicle (左锁骨)", ToolTip = "左侧锁骨骨骼。\n· 对应 UE5: clavicle_l\n· 对应 Mixamo: LeftShoulder\n· 对应 VRM: LeftShoulder"))
  FName l_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Shoulder / UpperArm (左大臂/上臂)", ToolTip = "左侧大臂骨骼。\n· 对应 UE5: upperarm_l\n· 对应 Mixamo: LeftArm\n· 对应 VRM: LeftUpperArm"))
  FName l_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Elbow / LowerArm (左小臂/前臂)", ToolTip = "左侧小臂骨骼。\n· 对应 UE5: lowerarm_l\n· 对应 Mixamo: LeftForeArm\n· 对应 VRM: LeftLowerArm"))
  FName l_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Wrist / Hand (左手腕/手掌)", ToolTip = "左手手腕/手掌主骨骼。\n· 对应 UE5: hand_l\n· 对应 Mixamo: LeftHand\n· 对应 VRM: LeftHand"))
  FName l_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Hand / Fingers (左手指/末端)", ToolTip = "左手末端/手指基础骨骼。\n· 对应 UE5: index_01_l\n· 对应 Mixamo: LeftHandIndex1"))
  FName l_hand_;

  // --- 3. 右上肢 (Right Arm) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Collar / Clavicle (右锁骨)", ToolTip = "右侧锁骨骨骼。\n· 对应 UE5: clavicle_r\n· 对应 Mixamo: RightShoulder\n· 对应 VRM: RightShoulder"))
  FName r_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Shoulder / UpperArm (右大臂/上臂)", ToolTip = "右侧大臂骨骼。\n· 对应 UE5: upperarm_r\n· 对应 Mixamo: RightArm\n· 对应 VRM: RightUpperArm"))
  FName r_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Elbow / LowerArm (右小臂/前臂)", ToolTip = "右侧小臂骨骼。\n· 对应 UE5: lowerarm_r\n· 对应 Mixamo: RightForeArm\n· 对应 VRM: RightLowerArm"))
  FName r_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Wrist / Hand (右手腕/手掌)", ToolTip = "右手手腕/手掌主骨骼。\n· 对应 UE5: hand_r\n· 对应 Mixamo: RightHand\n· 对应 VRM: RightHand"))
  FName r_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Hand / Fingers (右手指/末端)", ToolTip = "右手末端/手指基础骨骼。\n· 对应 UE5: index_01_r\n· 对应 Mixamo: RightHandIndex1"))
  FName r_hand_;

  // --- 4. 左下肢 (Left Leg) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Hip / Thigh / UpperLeg (左大腿)", ToolTip = "左侧大腿骨骼。\n· 对应 UE5: thigh_l\n· 对应 Mixamo: LeftUpLeg\n· 对应 VRM: LeftUpperLeg"))
  FName l_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Knee / Calf / LowerLeg (左小腿)", ToolTip = "左侧小腿骨骼。\n· 对应 UE5: calf_l\n· 对应 Mixamo: LeftLeg\n· 对应 VRM: LeftLowerLeg"))
  FName l_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Ankle / Foot (左脚踝/脚部)", ToolTip = "左侧脚踝骨骼。\n· 对应 UE5: foot_l\n· 对应 Mixamo: LeftFoot\n· 对应 VRM: LeftFoot"))
  FName l_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Foot / Ball / Toe (左脚趾/脚掌球)", ToolTip = "左脚脚尖/前掌骨骼。\n· 对应 UE5: ball_l\n· 对应 Mixamo: LeftToeBase\n· 对应 VRM: LeftToes"))
  FName l_foot_;

  // --- 5. 右下肢 (Right Leg) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Hip / Thigh / UpperLeg (右大腿)", ToolTip = "右侧大腿骨骼。\n· 对应 UE5: thigh_r\n· 对应 Mixamo: RightUpLeg\n· 对应 VRM: RightUpperLeg"))
  FName r_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Knee / Calf / LowerLeg (右小腿)", ToolTip = "右侧小腿骨骼。\n· 对应 UE5: calf_r\n· 对应 Mixamo: RightLeg\n· 对应 VRM: RightLowerLeg"))
  FName r_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Ankle / Foot (右脚踝/脚部)", ToolTip = "右侧脚踝骨骼。\n· 对应 UE5: foot_r\n· 对应 Mixamo: RightFoot\n· 对应 VRM: RightFoot"))
  FName r_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Foot / Ball / Toe (右脚趾/脚掌球)", ToolTip = "右脚脚尖/前掌骨骼。\n· 对应 UE5: ball_r\n· 对应 Mixamo: RightToeBase\n· 对应 VRM: RightToes"))
  FName r_foot_;
};