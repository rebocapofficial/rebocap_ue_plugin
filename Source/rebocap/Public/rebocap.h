// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "HAL/RunnableThread.h"
#include "rebocap_ws_sdk_cpp.h"

class FToolBarBuilder;
class FMenuBuilder;

class FrebocapModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:
	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply OnConnectButtonClicked();
	FReply OnOpenLiveLinkWindowClicked();
	FReply OnOpenDocsClicked();
	FReply OnOpenWebsiteClicked();

	FText GetConnectButtonText() const;
	FSlateColor GetStatusColor() const;
	FText GetStatusText() const;

	TOptional<uint16> GetPortValue() const;
	void OnPortChanged(uint16 value);

	void SetThreadPriority(EThreadPriority NewPriority);
	bool IsThreadPriority(EThreadPriority CheckPriority) const;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	uint16 ConnectPort = 7690;
	void* rebocap_ws_sdk_handle_ = nullptr;
};
