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

private:
  static bool bIsConnected;
};
