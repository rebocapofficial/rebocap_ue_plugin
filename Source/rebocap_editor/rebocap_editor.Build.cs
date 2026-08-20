using UnrealBuildTool;

public class rebocap_editor : ModuleRules
{
    public rebocap_editor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AnimGraphRuntime",
            "rebocap_runtime"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AnimGraph",
            "BlueprintGraph",
            "UnrealEd",
            "KismetCompiler",
            "GraphEditor",
            "PropertyEditor",
            "Slate",
            "SlateCore"
        });

    }
}
