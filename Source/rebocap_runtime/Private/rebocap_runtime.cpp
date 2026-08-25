#include "rebocap_runtime.h"
#include "rebocap_source.h"
#include "Interfaces/IPluginManager.h"
#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

void Frebocap_runtimeModule::StartupModule()
{
    FString BaseDir;
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("rebocap"));
    if (Plugin.IsValid())
    {
        BaseDir = Plugin->GetBaseDir();
    }
    else
    {
        BaseDir = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("rebocap"));
    }

    FString LibraryPath;
#if PLATFORM_WINDOWS
    TArray<FString> CandidatePaths;
    if (!BaseDir.IsEmpty())
    {
        CandidatePaths.Add(FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll")));
        CandidatePaths.Add(FPaths::Combine(*BaseDir, TEXT("x64/Release/rebocap_ws_sdk.dll")));
        CandidatePaths.Add(FPaths::Combine(*BaseDir, TEXT("Binaries/Win64/rebocap_ws_sdk.dll")));
        CandidatePaths.Add(FPaths::Combine(*BaseDir, TEXT("rebocap_ws_sdk.dll")));
    }
    CandidatePaths.Add(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("rebocap/Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll")));
    CandidatePaths.Add(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("rebocap/x64/Release/rebocap_ws_sdk.dll")));
    CandidatePaths.Add(FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("Marketplace/rebocap/Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll")));

    for (const FString& Path : CandidatePaths)
    {
        if (FPaths::FileExists(Path))
        {
            LibraryPath = Path;
            FPlatformProcess::AddDllDirectory(*FPaths::GetPath(Path));
            break;
        }
    }
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/testPulginLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/testPulginLibrary/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif 

    rebocap_ws_sdk_handle_ = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
    
    if (!rebocap_ws_sdk_handle_)
    {
        UE_LOG(LogTemp, Error, TEXT("========================================================================"));
        UE_LOG(LogTemp, Error, TEXT("[Rebocap] 严重警告: 无法加载动捕底层 SDK (rebocap_ws_sdk.dll)！"));
        UE_LOG(LogTemp, Error, TEXT("[Rebocap] 常见原因: 电脑缺少微软 Visual C++ 官方运行库 (vc_redist.x64)。"));
        UE_LOG(LogTemp, Error, TEXT("[Rebocap] 修复方案: 请下载并安装微软运行库合集: https://aka.ms/vs/17/release/vc_redist.x64.exe"));
        UE_LOG(LogTemp, Error, TEXT("========================================================================"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[Rebocap] Rebocap SDK DLL loaded successfully from %s"), *LibraryPath);
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