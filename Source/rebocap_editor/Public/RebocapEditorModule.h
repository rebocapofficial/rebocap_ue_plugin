#pragma once

#include "Modules/ModuleManager.h"

/**
 * Rebocap Editor Module
 */
class FRebocapEditorModule : public IModuleInterface
{
public:
    /** Called when the module is loaded into memory */
    virtual void StartupModule() override;

    /** Called before the module is unloaded */
    virtual void ShutdownModule() override;
};
