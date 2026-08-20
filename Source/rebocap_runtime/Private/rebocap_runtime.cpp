#include "rebocap_runtime.h"
#include "rebocap_source.h"
#include "Interfaces/IPluginManager.h"
#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

void Frebocap_runtimeModule::StartupModule()
{
    FString BaseDir = IPluginManager::Get().FindPlugin("rebocap")->GetBaseDir();
    FString LibraryPath;
#if PLATFORM_WINDOWS
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/testPulginLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/testPulginLibrary/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif 

    rebocap_ws_sdk_handle_ = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
    
    if (!rebocap_ws_sdk_handle_)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Rebocap SDK DLL at %s"), *LibraryPath);
    }
}

void Frebocap_runtimeModule::ShutdownModule()
{
    // 关键顺序：必须先销毁所有 SDK 实例，再释放 DLL 句柄，避免退出时调用已卸载 DLL 代码
    if (FRebocapSource::GetInstance().IsValid())
    {
        FRebocapSource::GetInstance()->ManualStop();
        FRebocapSource::SetInstance(nullptr);
    }

    if (rebocap_ws_sdk_handle_)
    {
        FPlatformProcess::FreeDllHandle(rebocap_ws_sdk_handle_);
        rebocap_ws_sdk_handle_ = nullptr;
    }
}

IMPLEMENT_MODULE(Frebocap_runtimeModule, rebocap_runtime)