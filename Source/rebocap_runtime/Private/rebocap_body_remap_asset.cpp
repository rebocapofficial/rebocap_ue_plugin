#include "rebocap_body_remap_asset.h"
#include "rebocap_skeleton_data.h"

URebocapMapData::URebocapMapData() {
  // 默认使用 UE5 Manny / Quinn 官方预设自动填入
  PresetTemplate = ERebocapBonePreset::UE5_Mannequin;
  ApplyPreset(ERebocapBonePreset::UE5_Mannequin);
}

void URebocapMapData::ApplyPreset(ERebocapBonePreset InPreset) {
  PresetTemplate = InPreset;

  switch (InPreset) {
    case ERebocapBonePreset::UE5_Mannequin:
      rebocap_pelvis_ = TEXT("pelvis");
      spine1_ = TEXT("spine_01");
      spine2_ = TEXT("spine_02");
      spine3_ = TEXT("spine_03");
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

    case ERebocapBonePreset::UE4_Mannequin:
      rebocap_pelvis_ = TEXT("pelvis");
      spine1_ = TEXT("spine_01");
      spine2_ = TEXT("spine_02");
      spine3_ = TEXT("spine_03");
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
