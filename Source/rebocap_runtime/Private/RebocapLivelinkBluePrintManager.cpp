#include "RebocapLivelinkBluePrintManager.h"

#include "rebocap_source.h"
#include "Features/IModularFeatures.h"
#include "LiveLinkClient.h"  
#include "ILiveLinkClient.h" 

bool URebocapLivelinkBluePrintManager::bIsConnected = false;

void URebocapLivelinkBluePrintManager::ConnectToRebocapLivelinkSource(int Port) {
  // Note: It's better for you to control livelink on your own ui system
  bIsConnected = true;
  if (!FRebocapSource::GetInstance().IsValid()) {
    UE_LOG(LogTemp, Display, TEXT("start create rebocap live link!!!!"));
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
  UE_LOG(LogTemp, Display, TEXT("DisconnectLiveLink!!!!"));
  if (FRebocapSource::GetInstance().IsValid()) {
    UE_LOG(LogTemp, Display, TEXT("start clear rebocap live link!!!!"));
    FRebocapSource::GetInstance()->ManualStop();
  }
}
