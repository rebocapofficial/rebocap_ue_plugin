// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
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

	FReply on_connect_button_click();

	// 获取连接按钮文本
	FText get_connect_button_text() const;

	// 获取连接按钮文本
	FText get_connect_error_text() const;

	// 获取连接端口号
	TOptional<uint16> get_port_value() const;

	void on_port_change(uint16 value);

	void on_pose_msg_callback(const QuatMsg* msg, rebocap::RebocapWsSdk* sdk);
	void on_exception_close_callback(rebocap::RebocapWsSdk* sdk);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	bool is_connect_ = false;
	uint16 connect_port_ = 7690;
	void* rebocap_ws_sdk_handle_;
	std::unique_ptr<rebocap::RebocapWsSdk> rebocap_sdk_;

	TOptional<bool> connect_ok_;
};
