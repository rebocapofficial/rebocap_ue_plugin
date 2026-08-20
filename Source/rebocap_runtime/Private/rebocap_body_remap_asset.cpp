#include "rebocap_body_remap_asset.h"
#include "rebocap_skeleton_data.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"

#if WITH_EDITOR
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/MessageDialog.h"
#endif

URebocapMapData::URebocapMapData() {
  // 默认使用 Unreal Engine (UE4 / UE5 / MetaHuman) 官方标准骨骼预设
  PresetTemplate = ERebocapBonePreset::Unreal_Engine;
  FootIndices.Init(-1, 12);
  ApplyPreset(ERebocapBonePreset::Unreal_Engine);
}

void URebocapMapData::ApplyPreset(ERebocapBonePreset InPreset) {
  PresetTemplate = InPreset;

  switch (InPreset) {
    case ERebocapBonePreset::Unreal_Engine:
      rebocap_pelvis_ = TEXT("pelvis");
      spine1_ = TEXT("spine_02"); // 腰椎 (spine) -> spine_02
      spine2_ = TEXT("spine_04"); // 胸腔 (chest) -> spine_04
      spine3_ = TEXT("spine_05"); // 上胸 (up chest) -> spine_05
      neck_ = TEXT("neck_01");
      head_ = TEXT("head");

      l_collar_ = TEXT("clavicle_l");
      l_shoulder_ = TEXT("upperarm_l");
      l_elbow_ = TEXT("lowerarm_l");
      l_wrist_ = TEXT("hand_l");
      l_hand_ = TEXT("index_01_l");

      r_collar_ = TEXT("clavicle_r");
      r_shoulder_ = TEXT("upperarm_r");
      r_elbow_ = TEXT("lowerarm_r");
      r_wrist_ = TEXT("hand_r");
      r_hand_ = TEXT("index_01_r");

      l_hip_ = TEXT("thigh_l");
      l_knee_ = TEXT("calf_l");
      l_ankle_ = TEXT("foot_l");
      l_foot_ = TEXT("ball_l");

      r_hip_ = TEXT("thigh_r");
      r_knee_ = TEXT("calf_r");
      r_ankle_ = TEXT("foot_r");
      r_foot_ = TEXT("ball_r");
      break;

    case ERebocapBonePreset::Mixamo:
      rebocap_pelvis_ = TEXT("Hips");
      spine1_ = TEXT("Spine");
      spine2_ = TEXT("Spine1");
      spine3_ = TEXT("Spine2");
      neck_ = TEXT("Neck");
      head_ = TEXT("Head");

      l_collar_ = TEXT("LeftShoulder");
      l_shoulder_ = TEXT("LeftArm");
      l_elbow_ = TEXT("LeftForeArm");
      l_wrist_ = TEXT("LeftHand");
      l_hand_ = TEXT("LeftHandIndex1");

      r_collar_ = TEXT("RightShoulder");
      r_shoulder_ = TEXT("RightArm");
      r_elbow_ = TEXT("RightForeArm");
      r_wrist_ = TEXT("RightHand");
      r_hand_ = TEXT("RightHandIndex1");

      l_hip_ = TEXT("LeftUpLeg");
      l_knee_ = TEXT("LeftLeg");
      l_ankle_ = TEXT("LeftFoot");
      l_foot_ = TEXT("LeftToeBase");

      r_hip_ = TEXT("RightUpLeg");
      r_knee_ = TEXT("RightLeg");
      r_ankle_ = TEXT("RightFoot");
      r_foot_ = TEXT("RightToeBase");
      break;

    case ERebocapBonePreset::Mixamo_Prefix:
      rebocap_pelvis_ = TEXT("mixamorig:Hips");
      spine1_ = TEXT("mixamorig:Spine");
      spine2_ = TEXT("mixamorig:Spine1");
      spine3_ = TEXT("mixamorig:Spine2");
      neck_ = TEXT("mixamorig:Neck");
      head_ = TEXT("mixamorig:Head");

      l_collar_ = TEXT("mixamorig:LeftShoulder");
      l_shoulder_ = TEXT("mixamorig:LeftArm");
      l_elbow_ = TEXT("mixamorig:LeftForeArm");
      l_wrist_ = TEXT("mixamorig:LeftHand");
      l_hand_ = TEXT("mixamorig:LeftHandIndex1");

      r_collar_ = TEXT("mixamorig:RightShoulder");
      r_shoulder_ = TEXT("mixamorig:RightArm");
      r_elbow_ = TEXT("mixamorig:RightForeArm");
      r_wrist_ = TEXT("mixamorig:RightHand");
      r_hand_ = TEXT("mixamorig:RightHandIndex1");

      l_hip_ = TEXT("mixamorig:LeftUpLeg");
      l_knee_ = TEXT("mixamorig:LeftLeg");
      l_ankle_ = TEXT("mixamorig:LeftFoot");
      l_foot_ = TEXT("mixamorig:LeftToeBase");

      r_hip_ = TEXT("mixamorig:RightUpLeg");
      r_knee_ = TEXT("mixamorig:RightLeg");
      r_ankle_ = TEXT("mixamorig:RightFoot");
      r_foot_ = TEXT("mixamorig:RightToeBase");
      break;

    case ERebocapBonePreset::MMD_Japanese:
      rebocap_pelvis_ = TEXT("センター");
      spine1_ = TEXT("下半身");
      spine2_ = TEXT("上半身");
      spine3_ = TEXT("上半身2");
      neck_ = TEXT("首");
      head_ = TEXT("頭");

      l_collar_ = TEXT("左肩");
      l_shoulder_ = TEXT("左腕");
      l_elbow_ = TEXT("左ひじ");
      l_wrist_ = TEXT("左手首");
      l_hand_ = TEXT("左人指１");

      r_collar_ = TEXT("右肩");
      r_shoulder_ = TEXT("右腕");
      r_elbow_ = TEXT("右ひじ");
      r_wrist_ = TEXT("右手首");
      r_hand_ = TEXT("右人指１");

      l_hip_ = TEXT("左足");
      l_knee_ = TEXT("左ひざ");
      l_ankle_ = TEXT("左足首");
      l_foot_ = TEXT("左つま先");

      r_hip_ = TEXT("右足");
      r_knee_ = TEXT("右ひざ");
      r_ankle_ = TEXT("右足首");
      r_foot_ = TEXT("右つま先");
      break;

    case ERebocapBonePreset::VRM_Humanoid:
      rebocap_pelvis_ = TEXT("Hips");
      spine1_ = TEXT("Spine");
      spine2_ = TEXT("Chest");
      spine3_ = TEXT("UpperChest");
      neck_ = TEXT("Neck");
      head_ = TEXT("Head");

      l_collar_ = TEXT("LeftShoulder");
      l_shoulder_ = TEXT("LeftUpperArm");
      l_elbow_ = TEXT("LeftLowerArm");
      l_wrist_ = TEXT("LeftHand");
      l_hand_ = TEXT("LeftIndexProximal");

      r_collar_ = TEXT("RightShoulder");
      r_shoulder_ = TEXT("RightUpperArm");
      r_elbow_ = TEXT("RightLowerArm");
      r_wrist_ = TEXT("RightHand");
      r_hand_ = TEXT("RightIndexProximal");

      l_hip_ = TEXT("LeftUpperLeg");
      l_knee_ = TEXT("LeftLowerLeg");
      l_ankle_ = TEXT("LeftFoot");
      l_foot_ = TEXT("LeftToes");

      r_hip_ = TEXT("RightUpperLeg");
      r_knee_ = TEXT("RightLowerLeg");
      r_ankle_ = TEXT("RightFoot");
      r_foot_ = TEXT("RightToes");
      break;

    case ERebocapBonePreset::Rebocap_Standard:
      rebocap_pelvis_ = rebocap_bones::pelvis;
      l_hip_ = rebocap_bones::l_hip;
      r_hip_ = rebocap_bones::r_hip;
      spine1_ = rebocap_bones::spine1;
      l_knee_ = rebocap_bones::l_knee;
      r_knee_ = rebocap_bones::r_knee;
      spine2_ = rebocap_bones::spine2;
      l_ankle_ = rebocap_bones::l_ankle;
      r_ankle_ = rebocap_bones::r_ankle;
      spine3_ = rebocap_bones::spine3;
      l_foot_ = rebocap_bones::l_foot;
      r_foot_ = rebocap_bones::r_foot;
      neck_ = rebocap_bones::neck;
      l_collar_ = rebocap_bones::l_collar;
      r_collar_ = rebocap_bones::r_collar;
      head_ = rebocap_bones::head;
      l_shoulder_ = rebocap_bones::l_shoulder;
      r_shoulder_ = rebocap_bones::r_shoulder;
      l_elbow_ = rebocap_bones::l_elbow;
      r_elbow_ = rebocap_bones::r_elbow;
      l_wrist_ = rebocap_bones::l_wrist;
      r_wrist_ = rebocap_bones::r_wrist;
      l_hand_ = rebocap_bones::l_hand;
      r_hand_ = rebocap_bones::r_hand;
      break;

    case ERebocapBonePreset::Custom:
    default:
      break;
  }

  InitializeTMap();
}

void URebocapMapData::ExportToJson() {
#if WITH_EDITOR
  IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
  if (!DesktopPlatform) {
    UE_LOG(LogTemp, Error, TEXT("DesktopPlatform unavailable for ExportToJson"));
    return;
  }

  const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
  const FString DefaultPath = FPaths::ProjectSavedDir();
  const FString DefaultFile = FString::Printf(TEXT("rebocap_bone_mapping_%s.json"), *GetName());
  TArray<FString> OutFilenames;

  const bool bSaved = DesktopPlatform->SaveFileDialog(
      ParentWindowHandle,
      TEXT("导出 Rebocap 骨骼映射为 JSON 文件 (完全兼容 Blender 格式)"),
      DefaultPath,
      DefaultFile,
      TEXT("JSON 文件 (*.json)|*.json"),
      EFileDialogFlags::None,
      OutFilenames);

  if (!bSaved || OutFilenames.Num() == 0) {
    return;
  }

  const FString SaveFilePath = OutFilenames[0];

  TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();

  // 1. 写入 24 根骨骼名称 (node_0 ~ node_23)
  RootObject->SetStringField(TEXT("node_0"), rebocap_pelvis_.ToString());
  RootObject->SetStringField(TEXT("node_1"), l_hip_.ToString());
  RootObject->SetStringField(TEXT("node_2"), r_hip_.ToString());
  RootObject->SetStringField(TEXT("node_3"), spine1_.ToString());
  RootObject->SetStringField(TEXT("node_4"), l_knee_.ToString());
  RootObject->SetStringField(TEXT("node_5"), r_knee_.ToString());
  RootObject->SetStringField(TEXT("node_6"), spine2_.ToString());
  RootObject->SetStringField(TEXT("node_7"), l_ankle_.ToString());
  RootObject->SetStringField(TEXT("node_8"), r_ankle_.ToString());
  RootObject->SetStringField(TEXT("node_9"), spine3_.ToString());
  RootObject->SetStringField(TEXT("node_10"), l_foot_.ToString());
  RootObject->SetStringField(TEXT("node_11"), r_foot_.ToString());
  RootObject->SetStringField(TEXT("node_12"), neck_.ToString());
  RootObject->SetStringField(TEXT("node_13"), l_collar_.ToString());
  RootObject->SetStringField(TEXT("node_14"), r_collar_.ToString());
  RootObject->SetStringField(TEXT("node_15"), head_.ToString());
  RootObject->SetStringField(TEXT("node_16"), l_shoulder_.ToString());
  RootObject->SetStringField(TEXT("node_17"), r_shoulder_.ToString());
  RootObject->SetStringField(TEXT("node_18"), l_elbow_.ToString());
  RootObject->SetStringField(TEXT("node_19"), r_elbow_.ToString());
  RootObject->SetStringField(TEXT("node_20"), l_wrist_.ToString());
  RootObject->SetStringField(TEXT("node_21"), r_wrist_.ToString());
  RootObject->SetStringField(TEXT("node_22"), l_hand_.ToString());
  RootObject->SetStringField(TEXT("node_23"), r_hand_.ToString());

  // 2. 写入脚底碰撞体顶点索引 (foot_idx_0 ~ foot_idx_11)
  for (int32 i = 0; i < 12; ++i) {
    const int32 IdxVal = FootIndices.IsValidIndex(i) ? FootIndices[i] : -1;
    RootObject->SetNumberField(FString::Printf(TEXT("foot_idx_%d"), i), IdxVal);
  }

  // 3. 序列化并保存
  FString OutputString;
  TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutputString);
  if (FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer)) {
    if (FFileHelper::SaveStringToFile(OutputString, *SaveFilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM)) {
      UE_LOG(LogTemp, Display, TEXT("成功导出 Rebocap 骨骼映射到 JSON: %s"), *SaveFilePath);
      FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("成功导出骨骼映射到文件：\n%s"), *SaveFilePath)));
    }
  }
#endif
}

void URebocapMapData::ImportFromJson() {
#if WITH_EDITOR
  IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
  if (!DesktopPlatform) {
    UE_LOG(LogTemp, Error, TEXT("DesktopPlatform unavailable for ImportFromJson"));
    return;
  }

  const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
  const FString DefaultPath = FPaths::ProjectSavedDir();
  TArray<FString> OutFilenames;

  const bool bOpened = DesktopPlatform->OpenFileDialog(
      ParentWindowHandle,
      TEXT("选择要导入的 Rebocap 骨骼映射 JSON 文件 (完全兼容 Blender 格式)"),
      DefaultPath,
      TEXT(""),
      TEXT("JSON 文件 (*.json)|*.json"),
      EFileDialogFlags::None,
      OutFilenames);

  if (!bOpened || OutFilenames.Num() == 0) {
    return;
  }

  const FString OpenFilePath = OutFilenames[0];
  FString JsonString;
  if (!FFileHelper::LoadFileToString(JsonString, *OpenFilePath)) {
    FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("读取文件失败：\n%s"), *OpenFilePath)));
    return;
  }

  TSharedPtr<FJsonObject> RootObject;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
  if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid()) {
    FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("解析 JSON 失败，文件格式不合法！")));
    return;
  }

  // 1. 读取 24 根骨骼名称 (node_0 ~ node_23)
  auto ReadNode = [&](const TCHAR* Key, FName& OutName) {
    if (RootObject->HasField(Key)) {
      OutName = FName(*RootObject->GetStringField(Key));
    }
  };

  ReadNode(TEXT("node_0"), rebocap_pelvis_);
  ReadNode(TEXT("node_1"), l_hip_);
  ReadNode(TEXT("node_2"), r_hip_);
  ReadNode(TEXT("node_3"), spine1_);
  ReadNode(TEXT("node_4"), l_knee_);
  ReadNode(TEXT("node_5"), r_knee_);
  ReadNode(TEXT("node_6"), spine2_);
  ReadNode(TEXT("node_7"), l_ankle_);
  ReadNode(TEXT("node_8"), r_ankle_);
  ReadNode(TEXT("node_9"), spine3_);
  ReadNode(TEXT("node_10"), l_foot_);
  ReadNode(TEXT("node_11"), r_foot_);
  ReadNode(TEXT("node_12"), neck_);
  ReadNode(TEXT("node_13"), l_collar_);
  ReadNode(TEXT("node_14"), r_collar_);
  ReadNode(TEXT("node_15"), head_);
  ReadNode(TEXT("node_16"), l_shoulder_);
  ReadNode(TEXT("node_17"), r_shoulder_);
  ReadNode(TEXT("node_18"), l_elbow_);
  ReadNode(TEXT("node_19"), r_elbow_);
  ReadNode(TEXT("node_20"), l_wrist_);
  ReadNode(TEXT("node_21"), r_wrist_);
  ReadNode(TEXT("node_22"), l_hand_);
  ReadNode(TEXT("node_23"), r_hand_);

  // 2. 读取脚底碰撞体顶点 (foot_idx_0 ~ foot_idx_11)
  FootIndices.SetNumUninitialized(12);
  for (int32 i = 0; i < 12; ++i) {
    const FString Key = FString::Printf(TEXT("foot_idx_%d"), i);
    if (RootObject->HasField(Key)) {
      FootIndices[i] = RootObject->GetIntegerField(Key);
    } else {
      FootIndices[i] = -1;
    }
  }

  // 3. 将预设模式自动设为自定义模式并更新映射表
  PresetTemplate = ERebocapBonePreset::Custom;
  InitializeTMap();
  MarkPackageDirty();

  UE_LOG(LogTemp, Display, TEXT("成功从 JSON 导入 Rebocap 骨骼映射: %s"), *OpenFilePath);
  FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("成功导入骨骼映射配置！\n共读取 24 根骨骼及 12 个脚底顶点。"))));
#endif
}

#if WITH_EDITOR
void URebocapMapData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
  Super::PostEditChangeProperty(PropertyChangedEvent);
  const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
  if (PropertyName == GET_MEMBER_NAME_CHECKED(URebocapMapData, PresetTemplate)) {
    if (PresetTemplate != ERebocapBonePreset::Custom) {
      ApplyPreset(PresetTemplate);
    }
  } else {
    InitializeTMap();
  }
}
#endif

void URebocapMapData::InitializeTMap() {
  name_mapping_.Empty();
  name_mapping_.Reserve(24);
  name_mapping_.Add(rebocap_bones::pelvis, rebocap_pelvis_);
  name_mapping_.Add(rebocap_bones::l_hip, l_hip_);
  name_mapping_.Add(rebocap_bones::r_hip, r_hip_);
  name_mapping_.Add(rebocap_bones::spine1, spine1_);
  name_mapping_.Add(rebocap_bones::l_knee, l_knee_);
  name_mapping_.Add(rebocap_bones::r_knee, r_knee_);
  name_mapping_.Add(rebocap_bones::spine2, spine2_);
  name_mapping_.Add(rebocap_bones::l_ankle, l_ankle_);
  name_mapping_.Add(rebocap_bones::r_ankle, r_ankle_);
  name_mapping_.Add(rebocap_bones::spine3, spine3_);
  name_mapping_.Add(rebocap_bones::l_foot, l_foot_);
  name_mapping_.Add(rebocap_bones::r_foot, r_foot_);
  name_mapping_.Add(rebocap_bones::neck, neck_);
  name_mapping_.Add(rebocap_bones::l_collar, l_collar_);
  name_mapping_.Add(rebocap_bones::r_collar, r_collar_);
  name_mapping_.Add(rebocap_bones::head, head_);
  name_mapping_.Add(rebocap_bones::l_shoulder, l_shoulder_);
  name_mapping_.Add(rebocap_bones::r_shoulder, r_shoulder_);
  name_mapping_.Add(rebocap_bones::l_elbow, l_elbow_);
  name_mapping_.Add(rebocap_bones::r_elbow, r_elbow_);
  name_mapping_.Add(rebocap_bones::l_wrist, l_wrist_);
  name_mapping_.Add(rebocap_bones::r_wrist, r_wrist_);
  name_mapping_.Add(rebocap_bones::l_hand, l_hand_);
  name_mapping_.Add(rebocap_bones::r_hand, r_hand_);
}

FName URebocapMapData::GetRemappedBoneName_Implementation(FName CurveName) const {
  if (auto it = name_mapping_.Find(CurveName)) {
    UE_LOG(LogTemp, Display, TEXT("GetRemappedBoneName_Implementation %s -> %s"), *CurveName.ToString(), *it->ToString());
    return *it;
  }
  UE_LOG(LogTemp, Warning, TEXT("GetRemappedBoneName_Implementation %s failed!"), *CurveName.ToString());
  return NAME_None;
}
