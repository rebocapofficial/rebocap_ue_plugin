using UnrealBuildTool;

public class rebocap_editor : ModuleRules
{
    public rebocap_editor(ReadOnlyTargetRules Target) : base(Target)
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

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AnimGraphRuntime",
            "LiveLinkInterface",
            "rebocap_runtime"
        });

        if (Target.Version.MajorVersion > 5 || (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 1))
        {
            PublicDependencyModuleNames.Add("LiveLinkAnimationCore");
        }
        else
        {
            PublicDependencyModuleNames.Add("LiveLink");
            PrivateDependencyModuleNames.Add("EditorStyle");
        }

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AnimGraph",
            "BlueprintGraph",
            "UnrealEd",
            "KismetCompiler",
            "GraphEditor",
            "PropertyEditor",
            "Slate",
            "SlateCore",
            "DesktopPlatform"
        });

        if (Target.Version.MajorVersion < 5)
        {
            PrivateDependencyModuleNames.Add("EditorStyle");
        }
    }
}
