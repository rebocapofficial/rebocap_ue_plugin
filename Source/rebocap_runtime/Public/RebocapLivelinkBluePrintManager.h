#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Features/IModularFeatures.h"
#include "RebocapLivelinkBluePrintManager.generated.h"

UCLASS()
class REBOCAP_RUNTIME_API URebocapLivelinkBluePrintManager : public UBlueprintFunctionLibrary {
  GENERATED_BODY()
public:
  /** 
   * 手动启动 Rebocap Live Link 动捕连接（适用于打包独立游戏或自定义游戏UI控制）。
   * @param Port Rebocap 动捕服务监听的本地 websocket 端口号（默认 7690）
   */
  UFUNCTION(BlueprintCallable, Category = "Rebocap|LiveLink", meta = (DisplayName = "Connect Rebocap LiveLink", ToolTip = "手动启动 Rebocap Live Link 动捕连接。"))
  static void ConnectToRebocapLivelinkSource(int Port = 7690);

  /** 
   * 手动断开当前 Rebocap Live Link 动捕连接并释放资源。
   */
  UFUNCTION(BlueprintCallable, Category = "Rebocap|LiveLink", meta = (DisplayName = "Disconnect Rebocap LiveLink", ToolTip = "手动断开当前 Rebocap Live Link 动捕连接并释放资源。"))
  static void DisconnectToRebocapLivelinkSource();

  /** 
   * 查询当前 Rebocap Live Link 动捕连接是否处于活跃接收状态。
   */
  UFUNCTION(BlueprintPure, Category = "Rebocap|LiveLink", meta = (DisplayName = "Is Rebocap Connected", ToolTip = "查询当前 Rebocap Live Link 动捕连接是否处于活跃接收状态。"))
  static bool IsConnectedToRebocap();

  /** 
   * 获取当前 Rebocap 动捕数据流的实际接收刷新率（Hz）。
   */
  UFUNCTION(BlueprintPure, Category = "Rebocap|Diagnostics", meta = (DisplayName = "Get Rebocap Mocap Frame Rate (Hz)", ToolTip = "获取当前 Rebocap 动捕数据流的实际接收刷新率（Hz）。"))
  static float GetMocapFrameRate();

  /** 
   * 启动 10 秒（或自定义时长）性能与诊断日志黑匣子采集。采集完成后会自动在 Saved/Logs 下生成报告并弹出所在文件夹。
   * @param DurationSeconds 采样录制时长（默认 10.0 秒）
   */
  UFUNCTION(BlueprintCallable, Category = "Rebocap|Diagnostics", meta = (DisplayName = "Start Rebocap Diagnostic Recording", ToolTip = "启动性能与诊断日志黑匣子采集，完成后自动导出分析报告并打开文件夹。"))
  static void StartDiagnosticRecording(float DurationSeconds = 10.0f);

  /** 
   * 查询当前是否正在进行诊断日志采集。
   */
  UFUNCTION(BlueprintPure, Category = "Rebocap|Diagnostics", meta = (DisplayName = "Is Rebocap Diagnostic Recording", ToolTip = "查询当前是否正在进行诊断日志采集。"))
  static bool IsDiagnosticRecording();

private:
  static bool bIsConnected;
};
