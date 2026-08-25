// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class rebocap_runtime : ModuleRules
{
    public rebocap_runtime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        if (Target.Version.MajorVersion < 5)
        {
            CppStandard = CppStandardVersion.Cpp17;
        }
        else
        {
            CppStandard = CppStandardVersion.Cpp20;
        }

        // 1. 头文件包含路径
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        // 2. 宏定义
        PublicDefinitions.Add("USE_REBOCAP_LIVELINK_MANAGER_DEMO=1");

        // 3. 链接第三方库 (Windows平台)
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // 库文件路径
            string LibPath = Path.Combine(PluginDirectory, "Binaries", "ThirdParty", "RebocapWsSdk", "Win64");
            
            // 链接 .lib
            PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "x64", "Release", "rebocap_ws_sdk.lib"));
            
            // 延迟加载 .dll
            PublicDelayLoadDLLs.Add("rebocap_ws_sdk.dll");

            // 确保打包时 .dll 会被拷贝过去
            RuntimeDependencies.Add(Path.Combine(LibPath, "rebocap_ws_sdk.dll"));
        }

        // 4. 公共模块依赖
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "LiveLinkInterface",
            "InputCore",
            "Slate",
            "SlateCore",
            "UMG"
        });

        if (Target.Version.MajorVersion >= 5)
        {
            PublicDependencyModuleNames.Add("LiveLinkAnimationCore"); // UE5 骨架数据模块
        }

        // 5. 私有模块依赖
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Projects",
            "Networking",
            "Sockets",
            "HTTP",
            "AnimGraphRuntime",
            "LiveLink",
            "LiveLinkMessageBusFramework",
            "Messaging",
            "Json",
            "JsonUtilities"
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "DesktopPlatform"
            });
        }
    }
}