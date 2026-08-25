// Copyright Epic Games, Inc. All Rights Reserved.

#include "rebocap.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "rebocapCommands.h"
#include "rebocapStyle.h"
#include "rebocap_source.h"
#include "rebocap_runtime.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"
#include "Interfaces/IPluginManager.h"
#include "Features/IModularFeatures.h"
#include "ILiveLinkClient.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformProcess.h"
#include "Styling/CoreStyle.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION >= 5
#include "Styling/AppStyle.h"
#define REBOCAP_STYLE_GET() FAppStyle::Get()
#else
#include "EditorStyleSet.h"
#define REBOCAP_STYLE_GET() FEditorStyle::Get()
#endif

static const FName rebocapTabName("rebocap");

#define LOCTEXT_NAMESPACE "FrebocapModule"

void FrebocapModule::StartupModule()
{
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
		.SetDisplayName(LOCTEXT("FrebocapTabTitle", "Rebocap Control Panel"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("rebocap"));
	if (Plugin.IsValid())
	{
		FString BaseDir = Plugin->GetBaseDir();
		FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/RebocapWsSdk/Win64/rebocap_ws_sdk.dll"));
		if (FPaths::FileExists(LibraryPath))
		{
			rebocap_ws_sdk_handle_ = FPlatformProcess::GetDllHandle(*LibraryPath);
		}
	}
}

void FrebocapModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FrebocapStyle::Shutdown();
	FrebocapCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(rebocapTabName);

	if (rebocap_ws_sdk_handle_)
	{
		FPlatformProcess::FreeDllHandle(rebocap_ws_sdk_handle_);
		rebocap_ws_sdk_handle_ = nullptr;
	}
}

TSharedRef<SDockTab> FrebocapModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBorder)
			.BorderImage(REBOCAP_STYLE_GET().GetBrush("ToolPanel.GroupBorder"))
			.Padding(12.0f)
			[
				SNew(SVerticalBox)

				// 1. 顶部 Header (标题与版本)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 0.0f, 0.0f, 10.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("RebocapTitle", "Rebocap 动捕控制中心"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(8.0f, 0.0f, 0.0f, 0.0f)
					.VAlign(VAlign_Center)
					[
						SNew(SBorder)
						.BorderImage(REBOCAP_STYLE_GET().GetBrush("Menu.Heading"))
						.Padding(FMargin(4.0f, 1.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("V2.0-beta07")))
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
							.ColorAndOpacity(FLinearColor(0.2f, 0.8f, 1.0f))
						]
					]
				]

				// 若 SDK 加载失败，显示醒目的一键修复横幅
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 0.0f, 0.0f, 8.0f)
				[
					SNew(SBorder)
					.Visibility_Lambda([]() {
						bool bLoaded = false;
						if (FModuleManager::Get().IsModuleLoaded("rebocap_runtime"))
						{
							bLoaded = Frebocap_runtimeModule::Get().IsSdkLoaded();
						}
						return bLoaded ? EVisibility::Collapsed : EVisibility::Visible;
					})
					.BorderBackgroundColor(FLinearColor(0.8f, 0.2f, 0.2f, 1.0f))
					.Padding(8.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(0.0f, 0.0f, 8.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("MissingVcRedist", "⚠️ 缺少微软 VC++ 运行库，动捕底层组件无法加载！"))
							.ColorAndOpacity(FLinearColor::Yellow)
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SButton)
							.Text(LOCTEXT("DownloadVcRedist", "一键下载安装微软运行库 (vc_redist.x64)"))
							.OnClicked_Lambda([]() {
								FPlatformProcess::LaunchURL(TEXT("https://aka.ms/vs/17/release/vc_redist.x64.exe"), nullptr, nullptr);
								return FReply::Handled();
							})
						]
					]
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 0.0f, 0.0f, 8.0f)
				[
					SNew(SSeparator)
				]

				// 2. 通信连接与端口设置
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					// 端口
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 6.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PortLabel", "通信端口 (Port):"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 12.0f, 0.0f)
					[
						SNew(SNumericEntryBox<uint16>)
						.IsEnabled(true)
						.AllowSpin(true)
						.MinSliderValue(1000)
						.MaxSliderValue(65535)
						.MinDesiredValueWidth(70)
						.Value_Raw(this, &FrebocapModule::GetPortValue)
						.OnValueChanged_Raw(this, &FrebocapModule::OnPortChanged)
					]
					// 连接按钮
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 8.0f, 0.0f)
					[
						SNew(SButton)
						.OnClicked_Raw(this, &FrebocapModule::OnConnectButtonClicked)
						.ContentPadding(FMargin(12.0f, 4.0f))
						[
							SNew(STextBlock)
							.Text_Raw(this, &FrebocapModule::GetConnectButtonText)
						]
					]
					// 状态指示
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(8.0f, 0.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text_Raw(this, &FrebocapModule::GetStatusText)
						.ColorAndOpacity_Raw(this, &FrebocapModule::GetStatusColor)
					]
				]

				// 3. ⚡ 线程调度优先级选择
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 8.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 8.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ThreadPriorityLabel", "线程调度优先级:"))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 4.0f, 0.0f)
					[
						SNew(SCheckBox)
						.Style(&REBOCAP_STYLE_GET().GetWidgetStyle<FCheckBoxStyle>("RadioButton"))
						.IsChecked_Lambda([this]() { return IsThreadPriority(TPri_Normal) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
						.OnCheckStateChanged_Lambda([this](ECheckBoxState State) { if (State == ECheckBoxState::Checked) SetThreadPriority(TPri_Normal); })
						[
							SNew(STextBlock).Text(LOCTEXT("PriNormal", "普通 (Normal)"))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 4.0f, 0.0f)
					[
						SNew(SCheckBox)
						.Style(&REBOCAP_STYLE_GET().GetWidgetStyle<FCheckBoxStyle>("RadioButton"))
						.IsChecked_Lambda([this]() { return IsThreadPriority(TPri_AboveNormal) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
						.OnCheckStateChanged_Lambda([this](ECheckBoxState State) { if (State == ECheckBoxState::Checked) SetThreadPriority(TPri_AboveNormal); })
						[
							SNew(STextBlock).Text(LOCTEXT("PriAboveNormal", "高优先级 (Above Normal)"))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(SCheckBox)
						.Style(&REBOCAP_STYLE_GET().GetWidgetStyle<FCheckBoxStyle>("RadioButton"))
						.IsChecked_Lambda([this]() { return IsThreadPriority(TPri_Highest) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
						.OnCheckStateChanged_Lambda([this](ECheckBoxState State) { if (State == ECheckBoxState::Checked) SetThreadPriority(TPri_Highest); })
						[
							SNew(STextBlock).Text(LOCTEXT("PriHighest", "最高优先级 (Highest)"))
						]
					]
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 8.0f)
				[
					SNew(SSeparator)
				]

				// 4. 快捷操作与入口
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0.0f, 0.0f, 8.0f, 0.0f)
					[
						SNew(SButton)
						.OnClicked_Raw(this, &FrebocapModule::OnOpenLiveLinkWindowClicked)
						.ContentPadding(FMargin(8.0f, 4.0f))
						[
							SNew(STextBlock)
							.Text(LOCTEXT("OpenLiveLinkBtn", "📡 打开 Live Link 窗口"))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0.0f, 0.0f, 8.0f, 0.0f)
					[
						SNew(SButton)
						.OnClicked_Raw(this, &FrebocapModule::OnOpenDocsClicked)
						.ContentPadding(FMargin(8.0f, 4.0f))
						[
							SNew(STextBlock)
							.Text(LOCTEXT("DocsBtn", "📚 官方开发文档"))
						]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.OnClicked_Raw(this, &FrebocapModule::OnOpenWebsiteClicked)
						.ContentPadding(FMargin(8.0f, 4.0f))
						[
							SNew(STextBlock)
							.Text(LOCTEXT("WebBtn", "🌐 Rebocap 官网"))
						]
					]
				]

				// 5. 底部使用技巧
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 10.0f, 0.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderImage(REBOCAP_STYLE_GET().GetBrush("ToolPanel.DarkGroupBorder"))
					.Padding(8.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Tip1", "💡 快速指引：在动画蓝图中添加 [Rebocap Pose Node] 和 [Rebocap A2T Node] 即可驱动全身动捕。"))
							.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 4.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Tip2", "💡 平滑插值：若在高刷显示器或独立窗口下感觉动作有微顿挫，可在 Pose 节点开启 [Enable Frame Interpolation] 获得极致丝滑。"))
							.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f))
						]
					]
				]
			]
		];
}

FReply FrebocapModule::OnConnectButtonClicked()
{
	auto Source = FRebocapSource::GetInstance();
	if (Source.IsValid() && Source->IsPortOpen())
	{
		Source->ManualStop();
	}
	else
	{
		IModularFeatures& ModularFeatures = IModularFeatures::Get();
		if (ModularFeatures.IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
		{
			ILiveLinkClient* LiveLinkClient = &ModularFeatures.GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
			if (LiveLinkClient)
			{
				if (!Source.IsValid())
				{
					Source = MakeShared<FRebocapSource>(ConnectPort);
					FRebocapSource::SetInstance(Source);
					LiveLinkClient->AddSource(Source);
				}
				else
				{
					Source->ManualStart(ConnectPort);
				}
			}
		}
	}
	return FReply::Handled();
}

FReply FrebocapModule::OnOpenLiveLinkWindowClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FTabId("LiveLinkClient"));
	return FReply::Handled();
}

FReply FrebocapModule::OnOpenDocsClicked()
{
	FPlatformProcess::LaunchURL(TEXT("https://docs.rebocap.com/"), nullptr, nullptr);
	return FReply::Handled();
}

FReply FrebocapModule::OnOpenWebsiteClicked()
{
	FPlatformProcess::LaunchURL(TEXT("https://www.rebocap.com/"), nullptr, nullptr);
	return FReply::Handled();
}

FText FrebocapModule::GetConnectButtonText() const
{
	auto Source = FRebocapSource::GetInstance();
	if (Source.IsValid() && Source->IsPortOpen())
	{
		return LOCTEXT("DisconnectBtn", "断开连接 (Disconnect)");
	}
	return LOCTEXT("ConnectBtn", "连接动捕 (Connect)");
}

FSlateColor FrebocapModule::GetStatusColor() const
{
	auto Source = FRebocapSource::GetInstance();
	if (Source.IsValid() && Source->IsPortOpen())
	{
		return FSlateColor(FLinearColor(0.1f, 0.9f, 0.2f)); // Green
	}
	return FSlateColor(FLinearColor(0.8f, 0.4f, 0.1f)); // Orange/Gray
}

FText FrebocapModule::GetStatusText() const
{
	auto Source = FRebocapSource::GetInstance();
	if (Source.IsValid() && Source->IsPortOpen())
	{
		return LOCTEXT("StatusConnected", "● 已连接 (Active)");
	}
	return LOCTEXT("StatusDisconnected", "○ 未连接 (Disconnected)");
}

TOptional<uint16> FrebocapModule::GetPortValue() const
{
	return ConnectPort;
}

void FrebocapModule::OnPortChanged(uint16 value)
{
	ConnectPort = value;
}

void FrebocapModule::SetThreadPriority(EThreadPriority NewPriority)
{
	auto Source = FRebocapSource::GetInstance();
	if (Source.IsValid())
	{
		Source->SetThreadPriority(NewPriority);
	}
}

bool FrebocapModule::IsThreadPriority(EThreadPriority CheckPriority) const
{
	auto Source = FRebocapSource::GetInstance();
	if (Source.IsValid())
	{
		return Source->GetThreadPriority() == CheckPriority;
	}
	return CheckPriority == TPri_Highest;
}

void FrebocapModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(rebocapTabName);
}

void FrebocapModule::RegisterMenus()
{
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