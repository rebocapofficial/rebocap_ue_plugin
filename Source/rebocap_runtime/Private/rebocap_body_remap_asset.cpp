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
    UE_LOG(LogTemp, Display, TEXT("GetRemappedBoneName_Implementation %s %s"), *CurveName.ToString(), *it->ToString());
    return *it;
  }
  UE_LOG(LogTemp, Warning, TEXT("GetRemappedBoneName_Implementation %s failed!"), *CurveName.ToString());
  return NAME_None; // ✅ 修正：返回 NAME_None 而不是 ""
}
