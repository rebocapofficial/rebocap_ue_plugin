#pragma once

#include "LiveLinkCustomVersion.h"
#include "LiveLinkRemapAsset.h"
#include "rebocap_body_remap_asset.generated.h"

/** 常用骨骼命名预设规范 */
UENUM(BlueprintType)
enum class ERebocapBonePreset : uint8 {
  Unreal_Engine UMETA(DisplayName = "Unreal Engine (UE4 / UE5 / MetaHuman 官方标准骨骼)"),
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
  ERebocapBonePreset PresetTemplate = ERebocapBonePreset::Unreal_Engine;

  /** 应用指定骨骼预设 */
  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void ApplyPreset(ERebocapBonePreset InPreset);

  /** 导出当前骨骼配置与脚底碰撞体顶点为 JSON 文件（完全兼容 Blender 插件格式） */
  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void ExportToJson();

  /** 从 JSON 文件导入骨骼配置与脚底碰撞体顶点（完全兼容 Blender 插件格式） */
  UFUNCTION(BlueprintCallable, Category = "BoneRemapping")
  void ImportFromJson();

#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "0. 映射表预览 (Mapping Preview)")
  TMap<FName, FName> name_mapping_;

  // --- 1. 躯干与头部 (Root & Spine & Head) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Pelvis / Hips (盆骨/根骨骼)", ToolTip = "动捕根骨骼。\n· 同时接收rebocap移动的坐标。"))
  FName rebocap_pelvis_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Spine1 / Spine (腰椎)", ToolTip = "通常对应肚脐这个关节。"))
  FName spine1_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Spine2 / Chest (胸腔)", ToolTip = "对应胃部的关节（胸腔三角凹槽处）\n· 控制整个胸腔的摆动"))
  FName spine2_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Spine3 / Up Chest (上胸)", ToolTip = "对应胸部/乳房的关节\n· 对于没有该关节的角色，该选项留空。"))
  FName spine3_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Neck / Neck_01 (脖子/颈部)", ToolTip = "脖子"))
  FName neck_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. 躯干与头部 (Root & Spine & Head)", meta = (DisplayName = "Head (头部)", ToolTip = "头部"))
  FName head_;

  // --- 2. 左上肢 (Left Arm) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Collar / Shoulder (左锁骨/肩膀)", ToolTip = "对应背书包的肩膀骨骼"))
  FName l_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Shoulder / UpperArm (左大臂/上臂)", ToolTip = "对应戴上臂追踪器的骨骼"))
  FName l_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Elbow / LowerArm (左小臂/前臂)", ToolTip = "对应戴下臂追踪器的骨骼"))
  FName l_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Wrist / Hand (左手腕/手掌)", ToolTip = "对应戴手追踪器的骨骼"))
  FName l_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "2. 左上肢 (Left Arm)", meta = (DisplayName = "Left Hand End / Fingers (左手指/末端)", ToolTip = "留空，暂无内容"))
  FName l_hand_;

  // --- 3. 右上肢 (Right Arm) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Collar / Shoulder (右锁骨/肩膀)", ToolTip = "对应背书包的肩膀骨骼"))
  FName r_collar_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Shoulder / UpperArm (右大臂/上臂)", ToolTip = "对应戴上臂追踪器的骨骼"))
  FName r_shoulder_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Elbow / LowerArm (右小臂/前臂)", ToolTip = "对应戴下臂追踪器的骨骼"))
  FName r_elbow_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Wrist / Hand (右手腕/手掌)", ToolTip = "对应戴手追踪器的骨骼"))
  FName r_wrist_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "3. 右上肢 (Right Arm)", meta = (DisplayName = "Right Hand End / Fingers (右手指/末端)", ToolTip = "留空，暂无内容"))
  FName r_hand_;

  // --- 4. 左下肢 (Left Leg) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Hip / Thigh / UpperLeg (左大腿)", ToolTip = "对应戴大腿追踪器的骨骼"))
  FName l_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Knee / Calf / LowerLeg (左小腿)", ToolTip = "对应戴小腿追踪器的骨骼"))
  FName l_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Ankle / Foot (左脚踝/脚部)", ToolTip = "对应戴脚追踪器的骨骼"))
  FName l_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4. 左下肢 (Left Leg)", meta = (DisplayName = "Left Foot End / Toe (左脚趾/脚掌球)", ToolTip = "留空，暂无内容"))
  FName l_foot_;

  // --- 5. 右下肢 (Right Leg) ---

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Hip / Thigh / UpperLeg (右大腿)", ToolTip = "对应戴大腿追踪器的骨骼"))
  FName r_hip_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Knee / Calf / LowerLeg (右小腿)", ToolTip = "对应戴小腿追踪器的骨骼"))
  FName r_knee_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Ankle / Foot (右脚踝/脚部)", ToolTip = "对应戴脚追踪器的骨骼"))
  FName r_ankle_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "5. 右下肢 (Right Leg)", meta = (DisplayName = "Right Foot End / Toe (右脚趾/脚掌球)", ToolTip = "留空，暂无内容"))
  FName r_foot_;

  // --- 6. 脚底贴地碰撞体顶点 (Foot Sole Indices - 内存存储用于兼容 Blender JSON，不显示在 UE 面板) ---
  UPROPERTY()
  TArray<int32> FootIndices;
};