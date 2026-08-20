#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Features/IModularFeatures.h"
#include "RebocapLivelinkBluePrintManager.generated.h"



UCLASS()
class REBOCAP_RUNTIME_API URebocapLivelinkBluePrintManager : public UBlueprintFunctionLibrary {
  GENERATED_BODY()
public:
  /** 
   * 手动启动 Rebocap Live Link 动捕连接。
   * @param Port Rebocap 动捕服务监听的本地 websocket 端口号（默认 7690）
   */
  UFUNCTION(BlueprintCallable, Category = "LiveLink", meta = (ToolTip = "手动启动 Rebocap Live Link 动捕连接。"))
  static void ConnectToRebocapLivelinkSource(int Port = 7690);

  /** 
   * 手动断开当前 Rebocap Live Link 动捕连接并释放资源。
   */
  UFUNCTION(BlueprintCallable, Category = "LiveLink", meta = (ToolTip = "手动断开当前 Rebocap Live Link 动捕连接并释放资源。"))
  static void DisconnectToRebocapLivelinkSource();

private:
  static bool bIsConnected;
};
