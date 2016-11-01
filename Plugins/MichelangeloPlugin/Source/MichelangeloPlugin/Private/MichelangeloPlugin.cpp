// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MichelangeloPluginPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "MichelangeloPluginStyle.h"
#include "MichelangeloPluginCommands.h"

#include "LevelEditor.h"

static const FName MichelangeloPluginTabName("MichelangeloPlugin");

#define LOCTEXT_NAMESPACE "FMichelangeloPluginModule"

using namespace Common;

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
	// Initialize curl:
	m_webAPI = std::make_unique<MichelangeloAPI::WebAPI>();

	auto emailEditableText =
		SNew(SEditableText)
		.HintText(FText::FromString("Username"))
		.OnTextCommitted_Raw(this, &FMichelangeloPluginModule::OnEmailTextCommitted);

	auto passwordEditableText =
		SNew(SEditableText)
		.HintText(FText::FromString("Password"))
		.IsPassword(true)
		.OnTextCommitted_Raw(this, &FMichelangeloPluginModule::OnPasswordTextCommitted);

	auto loginButton =
		SNew(SButton)
		.Text(FText::FromString("Login"))
		.OnClicked_Raw(this, &FMichelangeloPluginModule::LoginButtonClicked);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().VAlign(VAlign_Top)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().HAlign(HAlign_Left)
					[
						SNew(STextBlock).Text(FText::FromString("Email"))
					]
					+ SHorizontalBox::Slot().HAlign(HAlign_Left)
					[
						emailEditableText
					]
				]
				+ SVerticalBox::Slot().VAlign(VAlign_Bottom)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().HAlign(HAlign_Left)
					[
						SNew(STextBlock).Text(FText::FromString("Password"))
					]
					+ SHorizontalBox::Slot().HAlign(HAlign_Left)
					[
						passwordEditableText
					]
				]
				+ SVerticalBox::Slot().VAlign(VAlign_Bottom)
				[
					loginButton
				]
			]
		];
}

void FMichelangeloPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(MichelangeloPluginTabName);
}

FReply FMichelangeloPluginModule::LoginButtonClicked()
{
	auto email = Helpers::FStringToString(m_emailText.ToString());
	auto password = Helpers::FStringToString(m_passwordText.ToString());

	// TODO In case of failure display message
	// TODO remember me
	// TODO handle authentication result
	if(m_webAPI->Authenticate(email, password, false))
	{
		// TODO
	}
	else
	{
		// TODO
	}

	return FReply::Handled();
}

void FMichelangeloPluginModule::OnEmailTextCommitted(const FText& InText, ETextCommit::Type InCommitInfo)
{
	m_emailText = InText;
}
void FMichelangeloPluginModule::OnPasswordTextCommitted(const FText& InText, ETextCommit::Type InCommitInfo)
{
	m_passwordText = InText;
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