#include "rebocap_ws_sdk_cpp.h"
#include <vector>
#include <fstream>
#include "CoreMinimal.h"  
#include "Math/Vector.h" 

namespace rebocap {
RebocapWsSdk::RebocapWsSdk(CoordinateSpaceType coordinateSpaceType, bool globalQuat)
    : handle_(rebocap_ws_sdk_new(coordinateSpaceType, globalQuat), [](void *ptr) { rebocap_ws_sdk_release(ptr); }) {
  rebocap_ws_sdk_set_pose_msg_callback(handle_.get(), this, RebocapWsSdk::pose_msg_callback);
  rebocap_ws_sdk_set_exception_close_callback(handle_.get(), this, RebocapWsSdk::exception_close_callback);
}

// 🔽 新增析构函数，确保退出时注销回调
RebocapWsSdk::~RebocapWsSdk() {
  isShuttingDown = true;
  if (handle_) {
    rebocap_ws_sdk_set_pose_msg_callback(handle_.get(), nullptr, nullptr);
    rebocap_ws_sdk_set_exception_close_callback(handle_.get(), nullptr, nullptr);
    rebocap_ws_sdk_close(handle_.get());
  }
}

int RebocapWsSdk::Open(uint32_t port, std::string_view name, int64_t uid) {
  return rebocap_ws_sdk_open(handle_.get(), port, name.data(), name.size(), uid);
}

void RebocapWsSdk::Close() {
  isShuttingDown = true;
  if (handle_) {
    rebocap_ws_sdk_close(handle_.get());
  }
}

void RebocapWsSdk::UnregisterCallbacks() {
  if (handle_) {
    rebocap_ws_sdk_set_pose_msg_callback(handle_.get(), nullptr, nullptr);
    rebocap_ws_sdk_set_exception_close_callback(handle_.get(), nullptr, nullptr);
  }
}

void RebocapWsSdk::SetPoseMsgCallback(std::function<void(const struct QuatMsg *, RebocapWsSdk *)> &&func) {
  pose_msg_callback_ = std::move(func);
}

void RebocapWsSdk::SetExceptionCloseCallback(std::function<void(RebocapWsSdk *)> &&func) { exception_close_callback_ = std::move(func); }

void RebocapWsSdk::pose_msg_callback(const struct QuatMsg *msg, void *extra) {
  auto *that = reinterpret_cast<RebocapWsSdk *>(extra);
  if (!that) return;                // 防止空指针
  if (that->isShuttingDown) return; // 可在类里加一个 bool 标志位，表示正在关闭
  
  that->PoseMsgCallback(msg);
  if (that->pose_msg_callback_) {
    that->pose_msg_callback_(msg, that);
  }
}

void RebocapWsSdk::exception_close_callback(void *extra) {
  auto *that = reinterpret_cast<RebocapWsSdk *>(extra);
  if (!that) return;
  if (that->isShuttingDown) return;

  that->ExceptionCloseCallback();
  if (that->exception_close_callback_) {
  that->exception_close_callback_(that);
  }
}

int RebocapWsSdk::GetLastMsg(QuatMsg *msg) { return rebocap_ws_sdk_get_last_msg(handle_.get(), msg); }


inline void ConvertTArrayToStdVector(const TArray<FVector3f>& VecArray, std::vector<UV3f>& StdVec) {
  StdVec.clear();
  StdVec.reserve(VecArray.Num());

  for (const FVector3f& Vec : VecArray) {
    UV3f UV;
    UV.x = Vec.X;
    UV.y = Vec.Y;
    UV.z = Vec.Z;
    StdVec.push_back(UV);
  }
}

void save_to_file(const std::string& filename, const std::vector<UV3f>& temp_left_vertex, const std::vector<UV3f>& temp_left_normals, const std::vector<UV3f>& temp_right_vertex, const std::vector<UV3f>& temp_right_normals, const std::vector<UV3f>& temp_skeletons) {
  std::ofstream out(filename, std::ios::binary);
  std::vector<std::vector<UV3f>> data = {temp_left_vertex, temp_left_normals, temp_right_vertex, temp_right_normals, temp_skeletons};
  for (const auto& vec : data) {
    size_t size = vec.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    for (const auto& item : vec) {
      out.write(reinterpret_cast<const char*>(&item), sizeof(UV3f));
    }
  }
}

void load_from_file(const std::string& filename, std::vector<UV3f>& temp_left_vertex, std::vector<UV3f>& temp_left_normals, std::vector<UV3f>& temp_right_vertex, std::vector<UV3f>& temp_right_normals, std::vector<UV3f>& temp_skeletons) {
  std::ifstream in(filename, std::ios::binary);
  std::vector<std::vector<UV3f>*> data = {&temp_left_vertex, &temp_left_normals, &temp_right_vertex, &temp_right_normals, &temp_skeletons};
  for (auto& vec : data) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    vec->resize(size);
    for (size_t i = 0; i < size; ++i) {
      UV3f item{};
      in.read(reinterpret_cast<char*>(&item), sizeof(UV3f));
      (*vec)[i] = item;
    }
  }
}

int RebocapWsSdk::CalculateAndRegisterToRebocap(TArray<FVector3f> &left_vertex,
                                                TArray<FVector3f> &left_normals,
                                                TArray<FVector3f> &right_vertex,
                                                TArray<FVector3f> &right_normals,
                                                TArray<FVector3f> &skeletons,
                                                const std::string& coordinate,
                                                TArray<FVector3f> &foot_vertices) {
  std::vector<UV3f> temp_left_vertex, temp_left_normals, temp_right_vertex, temp_right_normals, temp_skeletons, out_foot_vertices;
  ConvertTArrayToStdVector(left_vertex, temp_left_vertex);
  ConvertTArrayToStdVector(left_normals, temp_left_normals);
  ConvertTArrayToStdVector(right_vertex, temp_right_vertex);
  ConvertTArrayToStdVector(right_normals, temp_right_normals);
  ConvertTArrayToStdVector(skeletons, temp_skeletons);
  ConvertTArrayToStdVector(foot_vertices, out_foot_vertices);

  // reserve for v0 version to debug for users
  // save_to_file("temp_foot_vert.data", temp_left_vertex, temp_left_normals, temp_right_vertex, temp_right_normals, temp_skeletons);

  const int calculate_foot_vertices = foot_vertices.Num() == 12 ? 0 : 1;
  if (calculate_foot_vertices) {
    out_foot_vertices.clear();
    out_foot_vertices.resize(12);
    foot_vertices.Empty();
  }

  const int Res = rebocap_ws_sdk_calculate_foot_vertex(
            handle_.get(),
            temp_left_vertex.data(), temp_left_vertex.size(),
            temp_right_vertex.data(), temp_right_vertex.size(),
            temp_left_normals.data(), temp_left_normals.size(),
            temp_right_normals.data(), temp_right_normals.size(),
            temp_skeletons.data(), coordinate.c_str(),
            out_foot_vertices.data(), calculate_foot_vertices
            );
  if (calculate_foot_vertices) {
    for (int i = 0; i < 12; i++) {
      foot_vertices.Add(FVector3f(out_foot_vertices[i].x, out_foot_vertices[i].y, out_foot_vertices[i].z));
    }
  }
  
  return Res;
}
}  // namespace rebocap