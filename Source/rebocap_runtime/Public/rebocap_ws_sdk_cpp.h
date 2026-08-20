#pragma once

#ifndef REBOCAP_EXPORT
#define REBOCAP_EXPORT
#endif
#include "rebocap_ws_sdk.h"
#include "CoreMinimal.h"        
#include "Math/Vector.h"        
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>    

#define REBOCAP_Pelvis_PARENTS_BONE -1
#define REBOCAP_L_Hip_PARENTS_BONE 0
#define REBOCAP_R_Hip_PARENTS_BONE 0
#define REBOCAP_Spine1_PARENTS_BONE 0
#define REBOCAP_L_Knee_PARENTS_BONE 1
#define REBOCAP_R_Knee_PARENTS_BONE 2
#define REBOCAP_Spine2_PARENTS_BONE 3
#define REBOCAP_L_Ankle_PARENTS_BONE 4
#define REBOCAP_R_Ankle_PARENTS_BONE 5
#define REBOCAP_Spine3_PARENTS_BONE 6
#define REBOCAP_L_Foot_PARENTS_BONE 7
#define REBOCAP_R_Foot_PARENTS_BONE 8
#define REBOCAP_Neck_PARENTS_BONE 9
#define REBOCAP_L_Collar_PARENTS_BONE 9
#define REBOCAP_R_Collar_PARENTS_BONE 9
#define REBOCAP_Head_PARENTS_BONE 12
#define REBOCAP_L_Shoulder_PARENTS_BONE 13
#define REBOCAP_R_Shoulder_PARENTS_BONE 14
#define REBOCAP_L_Elbow_PARENTS_BONE 16
#define REBOCAP_R_Elbow_PARENTS_BONE 17
#define REBOCAP_L_Wrist_PARENTS_BONE 18
#define REBOCAP_R_Wrist_PARENTS_BONE 19
#define REBOCAP_L_Hand_PARENTS_BONE 20
#define REBOCAP_R_Hand_PARENTS_BONE 21

#define REBOCAP_Pelvis_BONE 0
#define REBOCAP_L_Hip_BONE 1
#define REBOCAP_R_Hip_BONE 2
#define REBOCAP_Spine1_BONE 3
#define REBOCAP_L_Knee_BONE 4
#define REBOCAP_R_Knee_BONE 5
#define REBOCAP_Spine2_BONE 6
#define REBOCAP_L_Ankle_BONE 7
#define REBOCAP_R_Ankle_BONE 8
#define REBOCAP_Spine3_BONE 9
#define REBOCAP_L_Foot_BONE 10
#define REBOCAP_R_Foot_BONE 11
#define REBOCAP_Neck_BONE 12
#define REBOCAP_L_Collar_BONE 13
#define REBOCAP_R_Collar_BONE 14
#define REBOCAP_Head_BONE 15
#define REBOCAP_L_Shoulder_BONE 16
#define REBOCAP_R_Shoulder_BONE 17
#define REBOCAP_L_Elbow_BONE 18
#define REBOCAP_R_Elbow_BONE 19
#define REBOCAP_L_Wrist_BONE 20
#define REBOCAP_R_Wrist_BONE 21
#define REBOCAP_L_Hand_BONE 22
#define REBOCAP_R_Hand_BONE 23

namespace rebocap {

class REBOCAP_RUNTIME_API RebocapWsSdk {
 public:
  explicit RebocapWsSdk(CoordinateSpaceType coordinateSpaceType = DefaultCoordinate, bool globalQuat = false);

  // 🔽 析构函数只声明，不内联实现
  virtual ~RebocapWsSdk();

  int Open(uint32_t port, std::string_view name = "reborn_app", int64_t uid = 0);
  void Close();
  void UnregisterCallbacks();    // 新增：公共方法，用于注销回调

  int GetLastMsg(QuatMsg *msg);

  void SetPoseMsgCallback(std::function<void(const struct QuatMsg *, RebocapWsSdk *)> &&func);
  void SetExceptionCloseCallback(std::function<void(RebocapWsSdk *)> &&func);

  int CalculateAndRegisterToRebocap(TArray<FVector3f> &left_vertex,
                                    TArray<FVector3f> &left_normals,
                                    TArray<FVector3f> &right_vertex,
                                    TArray<FVector3f> &right_normals,
                                    TArray<FVector3f> &skeletons,
                                    const std::string& coordinate,
                                    TArray<FVector3f> &foot_vertices);

 private:
  virtual void PoseMsgCallback(const QuatMsg *msg) {}
  virtual void ExceptionCloseCallback() {}

  std::shared_ptr<void> handle_;
  std::function<void(const QuatMsg *, RebocapWsSdk *)> pose_msg_callback_;
  std::function<void(RebocapWsSdk *)> exception_close_callback_;

  // 🔽 新增关闭标志位
  bool isShuttingDown = false;

  static void pose_msg_callback(const QuatMsg *, void *extra);
  static void exception_close_callback(void *extra);
};
} // namespace rebocap