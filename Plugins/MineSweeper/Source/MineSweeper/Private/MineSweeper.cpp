// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeper.h"
#include "MineSweeperStyle.h"
#include "MineSweeperCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "ToolMenus.h"
#include "..\Public\MineSweeper.h"

DEFINE_LOG_CATEGORY(MineSweeperLog);

static const FName MineSweeperTabName("MineSweeper");

#define LOCTEXT_NAMESPACE "FMineSweeperModule"

void FMineSweeperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMineSweeperStyle::Initialize();
	FMineSweeperStyle::ReloadTextures();

	FMineSweeperCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMineSweeperCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMineSweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMineSweeperModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MineSweeperTabName, FOnSpawnTab::CreateRaw(this, &FMineSweeperModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMineSweeperTabTitle", "MineSweeper"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	m_GameUI = new FGameUI(m_GameInstance);
	m_OnClickDelegate = FOnClicked::CreateRaw(this, &FMineSweeperModule::OnClickDelegate);
}

FReply FMineSweeperModule::OnClickDelegate()
{
	//UpdateUI();
	return FReply::Handled();
}



void FMineSweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (m_GameUI)
	{
		delete m_GameUI;
	}

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMineSweeperStyle::Shutdown();

	FMineSweeperCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MineSweeperTabName);
}

void FMineSweeperModule::UpdateUI()
{


	FGlobalTabmanager::Get()->GetActiveTab()->SetContent(
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().
		AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Left)
		[
			m_GameUI->GetWindowDetails()
		]
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoHeight()
		[
			m_GameUI->ConstructBoard()
		]
	/*+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("HIT ME")))
			.OnClicked(m_OnClickDelegate)
		]*/
	]

	);

	return;

}
TSharedRef<SDockTab> FMineSweeperModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().
			AutoWidth()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Left)
				[
					m_GameUI->GetWindowDetails()
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					m_GameUI->ConstructBoard()
				]
				/*+ SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoHeight()
					[
						SNew(SButton)
						.Text(FText::FromString(TEXT("HIT ME")))
						.OnClicked(m_OnClickDelegate)
				]*/
			]
			
		];
}

void FMineSweeperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MineSweeperTabName);
}

void FMineSweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMineSweeperCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMineSweeperCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMineSweeperModule, MineSweeper)