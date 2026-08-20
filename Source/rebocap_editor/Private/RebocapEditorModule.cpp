#include "RebocapEditorModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FRebocapEditorModule, rebocap_editor)

void FRebocapEditorModule::StartupModule()
{
    UE_LOG(LogTemp, Display, TEXT("RebocapEditor module started"));
}

void FRebocapEditorModule::ShutdownModule()
{
    UE_LOG(LogTemp, Display, TEXT("RebocapEditor module shut down"));
}
