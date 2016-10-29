// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "MichelangeloPluginStyle.h"

class FMichelangeloPluginCommands : public TCommands<FMichelangeloPluginCommands>
{
public:

	FMichelangeloPluginCommands()
		: TCommands<FMichelangeloPluginCommands>(TEXT("MichelangeloPlugin"), NSLOCTEXT("Contexts", "MichelangeloPlugin", "MichelangeloPlugin Plugin"), NAME_None, FMichelangeloPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};