#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class Frebocap_runtimeModule : public IModuleInterface
{
public:
  static inline Frebocap_runtimeModule& Get()
  {
    return FModuleManager::LoadModuleChecked<Frebocap_runtimeModule>("rebocap_runtime");
  }

  static inline bool IsAvailable()
  {
    return FModuleManager::Get().IsModuleLoaded("rebocap_runtime");
  }

  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

private:
  void* rebocap_ws_sdk_handle_ = nullptr;
};