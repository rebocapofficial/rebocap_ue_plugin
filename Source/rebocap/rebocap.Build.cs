// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class rebocap : ModuleRules
{
	public rebocap(ReadOnlyTargetRules Target) : base(Target)
	{
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = false;
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        // PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "x64", "Release", "rebocap_ws_sdk.lib"));
        //
        // // Delay-load the DLL, so we can load it from the right place first
        // PublicDelayLoadDLLs.Add("rebocap_ws_sdk.dll");
        //
        // // Ensure that the DLL is staged along with the executable
        // RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll");

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				// Path.Combine(ModuleDirectory, "..", "rebocap_runtime", "Public"),
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				// Path.Combine(ModuleDirectory, "..", "rebocap_runtime", "Private"),
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"rebocap_runtime",
				"AnimGraph",
				"AnimGraphRuntime",
				"BlueprintGraph",
				"Core",
				"LiveLinkAnimationCore",
                "LiveLinkInterface",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				
				"GraphEditor",
				"UnrealEd",
				"ToolMenus",
                "LiveLinkInterface",
                "Sockets",
                "Networking",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
