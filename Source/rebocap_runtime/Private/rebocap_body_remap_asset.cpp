#include "rebocap_body_remap_asset.h"
#include "rebocap_skeleton_data.h"

URebocapMapData::URebocapMapData() {
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
}

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

// ----------------------------------------------------------------------------
// 预设 1: UE5 Manny / Quinn (官方标准骨骼) 预设
// ----------------------------------------------------------------------------
URebocapMapData_UE5Mannequin::URebocapMapData_UE5Mannequin() {
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
}

// ----------------------------------------------------------------------------
// 预设 2: UE4 SK_Mannequin (经典小白人骨骼) 预设
// ----------------------------------------------------------------------------
URebocapMapData_UE4Mannequin::URebocapMapData_UE4Mannequin() {
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
}

// ----------------------------------------------------------------------------
// 预设 3: Mixamo 骨骼规范预设
// ----------------------------------------------------------------------------
URebocapMapData_Mixamo::URebocapMapData_Mixamo() {
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
}

// ----------------------------------------------------------------------------
// 预设 4: VRM / VRoid / Unity Humanoid 骨骼规范预设
// ----------------------------------------------------------------------------
URebocapMapData_VRM_Humanoid::URebocapMapData_VRM_Humanoid() {
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
}
