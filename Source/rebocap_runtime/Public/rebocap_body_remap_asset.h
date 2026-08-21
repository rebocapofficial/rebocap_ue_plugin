#pragma once

#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"
#include "rebocap_body_remap_asset.generated.h"

/** Bone Naming Preset Standards */
UENUM(BlueprintType)
enum class ERebocapBonePreset : uint8 {
  Unreal_Engine UMETA(DisplayName = "Unreal Engine (UE4 / UE5 / MetaHuman Standard)"),
  Mixamo UMETA(DisplayName = "Adobe Mixamo (No Prefix: Hips, Spine...)"),
  Mixamo_Prefix UMETA(DisplayName = "Adobe Mixamo (With Prefix: mixamorig:Hips...)"),
  MMD_Japanese UMETA(DisplayName = "MMD Standard Japanese Bones (センター, 上半身, 左腕...)"),
  VRM_Humanoid UMETA(DisplayName = "VRM / VRoid / Unity Humanoid Standard"),
  Rebocap_Standard UMETA(DisplayName = "Rebocap 24 Bones Standard (Default)"),
  Custom UMETA(DisplayName = "Custom")
};

/** 
 * Rebocap Bone Remapping Asset
 * Maps standard Rebocap 24-bone motion capture nodes to the character's skeletal joint names.
 */
UCLASS(BlueprintType, Blueprintable, meta = (ToolTip = "Rebocap bone mapping asset for retargeting mocap bones to character skeleton joint names. / Rebocap 骨骼映射资产。"))
class REBOCAP_RUNTIME_API URebocapMapData : public ULiveLinkRemapAsset {
  GENERATED_BODY()

 public:
  URebocapMapData();

  FName GetRemappedBoneName_Implementation(FName CurveName) const final;

  void Initialize() final { InitializeTMap(); }

  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void InitializeTMap();

  /** Preset Template */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Presets & Actions", meta = (DisplayName = "Select Bone Preset", ToolTip = "Select a bone preset to automatically populate all joint names below. / 选择骨骼预设，自动填充下方骨骼名称。"))
  ERebocapBonePreset PresetTemplate = ERebocapBonePreset::Unreal_Engine;

  /** Apply Bone Preset */
  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void ApplyPreset(ERebocapBonePreset InPreset);

  /** Export to JSON */
  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void ExportToJson();

  /** Import from JSON */
  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void ImportFromJson();

#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "0. Mapping Preview", meta = (DisplayName = "Name Mapping"))
  TMap<FName, FName> name_mapping_;

  // --- 1. Root & Spine & Head ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Root & Spine & Head", meta = (DisplayName = "Pelvis / Hips", ToolTip = "Root mocap bone, receives root motion translation. / 动捕根骨骼，接收根位移。"))
  FName rebocap_pelvis_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Root & Spine & Head", meta = (DisplayName = "Spine1 / Waist", ToolTip = "Waist / Lower spine joint. / 腰椎骨骼。"))
  FName spine1_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Root & Spine & Head", meta = (DisplayName = "Spine2 / Chest", ToolTip = "Chest joint for mid-torso bend. / 胸腔骨骼。"))
  FName spine2_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Root & Spine & Head", meta = (DisplayName = "Spine3 / Up Chest", ToolTip = "Upper chest joint (leave empty if skeleton has no spine3). / 上胸骨骼。"))
  FName spine3_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Root & Spine & Head", meta = (DisplayName = "Neck", ToolTip = "Neck joint. / 脖子/颈部。"))
  FName neck_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Root & Spine & Head", meta = (DisplayName = "Head", ToolTip = "Head joint. / 头部骨骼。"))
  FName head_;

  // --- 2. Left Arm ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Left Arm", meta = (DisplayName = "Left Collar / Clavicle", ToolTip = "Left shoulder / clavicle joint. / 左锁骨/肩膀骨骼。"))
  FName l_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Left Arm", meta = (DisplayName = "Left Shoulder / UpperArm", ToolTip = "Left upper arm joint. / 左大臂骨骼。"))
  FName l_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Left Arm", meta = (DisplayName = "Left Elbow / LowerArm", ToolTip = "Left forearm / lower arm joint. / 左小臂骨骼。"))
  FName l_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Left Arm", meta = (DisplayName = "Left Wrist / Hand", ToolTip = "Left wrist / hand joint. / 左手腕/手掌骨骼。"))
  FName l_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. Left Arm", meta = (DisplayName = "Left Hand End / Fingers", ToolTip = "Left hand end / fingers joint (optional). / 左手指/末端。"))
  FName l_hand_;

  // --- 3. Right Arm ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. Right Arm", meta = (DisplayName = "Right Collar / Clavicle", ToolTip = "Right shoulder / clavicle joint. / 右锁骨/肩膀骨骼。"))
  FName r_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. Right Arm", meta = (DisplayName = "Right Shoulder / UpperArm", ToolTip = "Right upper arm joint. / 右大臂骨骼。"))
  FName r_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. Right Arm", meta = (DisplayName = "Right Elbow / LowerArm", ToolTip = "Right forearm / lower arm joint. / 右小臂骨骼。"))
  FName r_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. Right Arm", meta = (DisplayName = "Right Wrist / Hand", ToolTip = "Right wrist / hand joint. / 右手腕/手掌骨骼。"))
  FName r_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. Right Arm", meta = (DisplayName = "Right Hand End / Fingers", ToolTip = "Right hand end / fingers joint (optional). / 右手指/末端。"))
  FName r_hand_;

  // --- 4. Left Leg ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. Left Leg", meta = (DisplayName = "Left Hip / Thigh", ToolTip = "Left thigh / upper leg joint. / 左大腿骨骼。"))
  FName l_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. Left Leg", meta = (DisplayName = "Left Knee / Calf", ToolTip = "Left knee / lower leg joint. / 左小腿骨骼。"))
  FName l_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. Left Leg", meta = (DisplayName = "Left Ankle / Foot", ToolTip = "Left ankle / foot joint. / 左脚踝骨骼。"))
  FName l_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. Left Leg", meta = (DisplayName = "Left Foot End / Toe", ToolTip = "Left toe joint (optional). / 左脚趾。"))
  FName l_foot_;

  // --- 5. Right Leg ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. Right Leg", meta = (DisplayName = "Right Hip / Thigh", ToolTip = "Right thigh / upper leg joint. / 右大腿骨骼。"))
  FName r_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. Right Leg", meta = (DisplayName = "Right Knee / Calf", ToolTip = "Right knee / lower leg joint. / 右小腿骨骼。"))
  FName r_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. Right Leg", meta = (DisplayName = "Right Ankle / Foot", ToolTip = "Right ankle / foot joint. / 右脚踝骨骼。"))
  FName r_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. Right Leg", meta = (DisplayName = "Right Foot End / Toe", ToolTip = "Right toe joint (optional). / 右脚趾。"))
  FName r_foot_;

  // --- 6. Foot Sole Collision Indices ---
  UPROPERTY()
  TArray<int32> FootIndices;
};