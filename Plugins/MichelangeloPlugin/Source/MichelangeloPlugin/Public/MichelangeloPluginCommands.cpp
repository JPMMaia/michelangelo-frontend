// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MichelangeloPluginPrivatePCH.h"
#include "MichelangeloPluginCommands.h"

#define LOCTEXT_NAMESPACE "FMichelangeloPluginModule"

void FMichelangeloPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MichelangeloPlugin", "Bring up MichelangeloPlugin window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
