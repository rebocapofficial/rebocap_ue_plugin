// Copyright Epic Games, Inc. All Rights Reserved.

#include "rebocap.h"

#include "LevelEditor.h"
#include "rebocapCommands.h"
#include "rebocapStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SBox.h"
#include "Interfaces/IPluginManager.h"

static const FName rebocapTabName("rebocap");

#define LOCTEXT_NAMESPACE "FrebocapModule"

void FrebocapModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FrebocapStyle::Initialize();
	FrebocapStyle::ReloadTextures();

	FrebocapCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FrebocapCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FrebocapModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FrebocapModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(rebocapTabName, FOnSpawnTab::CreateRaw(this, &FrebocapModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FrebocapTabTitle", "rebocap"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FString BaseDir = IPluginManager::Get().FindPlugin("rebocap")->GetBaseDir();

	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll"));
#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/testPulginLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/testPulginLibrary/Linux/x86_64-unknown-linux-gnu/libExampleLibrary.so"));
#endif // PLATFORM_WINDOWS

	rebocap_ws_sdk_handle_ = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	
	// rebocap_sdk_ = std::make_unique<rebocap::RebocapWsSdk>(DefaultCoordinate, true);
	// rebocap_sdk_->SetPoseMsgCallback([this](const QuatMsg* msg, rebocap::RebocapWsSdk* sdk) {
	// 	on_pose_msg_callback(msg, sdk);
	// });
	// rebocap_sdk_->SetExceptionCloseCallback([this](rebocap::RebocapWsSdk* sdk) {
	// 	on_exception_close_callback(sdk);
	// });
}

void FrebocapModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FrebocapStyle::Shutdown();

	FrebocapCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(rebocapTabName);

	FPlatformProcess::FreeDllHandle(rebocap_ws_sdk_handle_);
	rebocap_ws_sdk_handle_ = nullptr;
}

TSharedRef<SDockTab> FrebocapModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add ?? code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FrebocapModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("rebocap.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(10.0f, 3.0f, 10.0f, 3.0f)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Connect Port")))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SNumericEntryBox<uint16>)
					.IsEnabled(true)
					.AllowSpin(true)
					.MinSliderValue(0)
					.MaxSliderValue(65535)
					.MinDesiredValueWidth(75)
					.Value_Raw(this, &FrebocapModule::get_port_value)
					.OnValueChanged_Raw(this, &FrebocapModule::on_port_change)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10.0f, 0, 0, 10.f)
				[
					SNew(SButton)
					.Text_Raw(this, &FrebocapModule::get_connect_button_text)
					.OnClicked_Raw(this, &FrebocapModule::on_connect_button_click)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text_Raw(this, &FrebocapModule::get_connect_error_text)
				]
			]
		];
}

FReply FrebocapModule::on_connect_button_click() {
	UE_LOG(LogTemp, Display, TEXT("Button Clicked???????!"));
	// if (!is_connect_) {
	// 	auto open_res = rebocap_sdk_->Open(connect_port_);
	// 	connect_ok_ = open_res == 0 ? true : false;
	// 	if (connect_ok_.GetValue()) {
	// 		is_connect_ = true;
	// 	}
	// }
	// else {
	// 	rebocap_sdk_->Close();
	// 	connect_ok_.Reset();
	// 	is_connect_ = false;
	// }
	return FReply::Handled();
}

FText FrebocapModule::get_connect_button_text() const {
	// if (is_connect_) {
	// 	return FText::FromString(TEXT("Disconnect"));
	// }
    return FText::FromString(TEXT("Connect"));
}

FText FrebocapModule::get_connect_error_text() const {
	// if (connect_ok_.IsSet() && !connect_ok_.GetValue()) {
	// 	return FText::FromString(TEXT("Connect Fail"));
	// }
	return FText::FromString("");
}

TOptional<uint16> FrebocapModule::get_port_value() const {
	return connect_port_;
}

void FrebocapModule::on_port_change(uint16 value) {
	connect_port_ = value;
}

void FrebocapModule::on_pose_msg_callback(const QuatMsg* msg, rebocap::RebocapWsSdk* sdk) {
	// UE_LOG(LogTemp, Display, TEXT("trans=%f %f %f"), msg->trans[0], msg->trans[1], msg->trans[2]);
	// for (size_t i = 0; i < 96; i++) {
	// 	UE_LOG(LogTemp, Warning, TEXT("quat[%d]=%f"), i, msg->quat[i]);
	// }
}

void FrebocapModule::on_exception_close_callback(rebocap::RebocapWsSdk* sdk) {
	// is_connect_ = false;
	// connect_ok_.Reset();
}

void FrebocapModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(rebocapTabName);
}

void FrebocapModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FrebocapCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FrebocapCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FrebocapModule, rebocap)