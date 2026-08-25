#include "RebocapLivelinkBluePrintManager.h"

#include "rebocap_source.h"
#include "Features/IModularFeatures.h"
#include "LiveLinkClient.h"  
#include "ILiveLinkClient.h" 

bool URebocapLivelinkBluePrintManager::bIsConnected = false;

void URebocapLivelinkBluePrintManager::ConnectToRebocapLivelinkSource(int Port) {
  bIsConnected = true;
  if (!FRebocapSource::GetInstance().IsValid()) {
    UE_LOG(LogTemp, Display, TEXT("[Rebocap] Starting and creating Rebocap LiveLink source on port: %d"), Port);
    // 创建新的 LiveLink Source
    TSharedPtr<FRebocapSource> NewSource = MakeShared<FRebocapSource>(static_cast<uint16_t>(Port));

    // 获取 LiveLink Client
    IModularFeatures& ModularFeatures = IModularFeatures::Get();
    if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName)) {
      ILiveLinkClient* LiveLinkClient = &ModularFeatures.GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

      if (LiveLinkClient) {
        // 将 Source 添加到 LiveLink Client
        FRebocapSource::SetInstance(NewSource);
        LiveLinkClient->AddSource(NewSource);
      }
    }
  } else {
    FRebocapSource::GetInstance()->ManualStart(Port);
  }
}

void URebocapLivelinkBluePrintManager::DisconnectToRebocapLivelinkSource() {
  bIsConnected = false;
  UE_LOG(LogTemp, Display, TEXT("[Rebocap] Disconnecting Rebocap LiveLink source..."));
  if (FRebocapSource::GetInstance().IsValid()) {
    FRebocapSource::GetInstance()->ManualStop();
  }
}

bool URebocapLivelinkBluePrintManager::IsConnectedToRebocap() {
  auto Source = FRebocapSource::GetInstance();
  if (Source.IsValid()) {
    return Source->IsPortOpen();
  }
  return false;
}
