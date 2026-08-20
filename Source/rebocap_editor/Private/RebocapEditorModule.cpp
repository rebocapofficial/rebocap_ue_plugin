#include "RebocapEditorModule.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "RebocapMapDataDetails.h"
#include "rebocap_body_remap_asset.h"

IMPLEMENT_MODULE(FRebocapEditorModule, rebocap_editor)

void FRebocapEditorModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(
        URebocapMapData::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FRebocapMapDataDetails::MakeInstance)
    );

    UE_LOG(LogTemp, Display, TEXT("RebocapEditor module started and registered custom details"));
}

void FRebocapEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.UnregisterCustomClassLayout(URebocapMapData::StaticClass()->GetFName());
    }

    UE_LOG(LogTemp, Display, TEXT("RebocapEditor module shut down"));
}
