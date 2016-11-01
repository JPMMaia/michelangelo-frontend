// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "WebAPI.h"

#include <memory>

class FToolBarBuilder;
class FMenuBuilder;

class FMichelangeloPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	FReply LoginButtonClicked();
	void OnEmailTextCommitted(const FText& InText, ETextCommit::Type InCommitInfo);
	void OnPasswordTextCommitted(const FText& InText, ETextCommit::Type InCommitInfo);
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	std::unique_ptr<MichelangeloAPI::WebAPI> m_webAPI;
	FText m_emailText;
	FText m_passwordText;
};