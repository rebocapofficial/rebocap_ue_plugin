#include "RebocapEditorModule.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "RebocapMapDataDetails.h"
#include "RebocapA2TDetails.h"
#include "rebocap_body_remap_asset.h"
#include "AnimGraphNode_RebocapA2T.h"

IMPLEMENT_MODULE(FRebocapEditorModule, rebocap_editor)

void FRebocapEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    // 1. 骨骼映射资产细节面板自定义 (JSON 导入/导出)
    PropertyModule.RegisterCustomClassLayout(
        URebocapMapData::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FRebocapMapDataDetails::MakeInstance)
    );

    // 2. A2T 姿态校准节点细节面板自定义 (JSON 导入/导出)
    PropertyModule.RegisterCustomClassLayout(
        UAnimGraphNode_RebocapA2T::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FRebocapA2TDetails::MakeInstance)
    );

    UE_LOG(LogTemp, Display, TEXT("RebocapEditor module started and registered custom details"));
}

void FRebocapEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout(URebocapMapData::StaticClass()->GetFName());
        PropertyModule.UnregisterCustomClassLayout(UAnimGraphNode_RebocapA2T::StaticClass()->GetFName());
    }

    UE_LOG(LogTemp, Display, TEXT("RebocapEditor module shut down"));
}

