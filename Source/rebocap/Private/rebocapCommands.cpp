// Copyright Epic Games, Inc. All Rights Reserved.

#include "rebocapCommands.h"

#define LOCTEXT_NAMESPACE "FrebocapModule"

void FrebocapCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "rebocap", "Bring up rebocap window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
