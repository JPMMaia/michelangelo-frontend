// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MichelangeloPluginPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "MichelangeloPluginStyle.h"
#include "MichelangeloPluginCommands.h"

#include "LevelEditor.h"

static const FName MichelangeloPluginTabName("MichelangeloPlugin");

#define LOCTEXT_NAMESPACE "FMichelangeloPluginModule"

void FMichelangeloPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMichelangeloPluginStyle::Initialize();
	FMichelangeloPluginStyle::ReloadTextures();

	FMichelangeloPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMichelangeloPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMichelangeloPluginModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FMichelangeloPluginModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FMichelangeloPluginModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MichelangeloPluginTabName, FOnSpawnTab::CreateRaw(this, &FMichelangeloPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMichelangeloPluginTabTitle", "MichelangeloPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMichelangeloPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FMichelangeloPluginStyle::Shutdown();

	FMichelangeloPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MichelangeloPluginTabName);
}

TSharedRef<SDockTab> FMichelangeloPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMichelangeloPluginModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("MichelangeloPlugin.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FMichelangeloPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(MichelangeloPluginTabName);
}

void FMichelangeloPluginModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FMichelangeloPluginCommands::Get().OpenPluginWindow);
}

void FMichelangeloPluginModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FMichelangeloPluginCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMichelangeloPluginModule, MichelangeloPlugin)