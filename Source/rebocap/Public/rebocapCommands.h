// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "rebocapStyle.h"

class FrebocapCommands : public TCommands<FrebocapCommands>
{
public:

	FrebocapCommands()
		: TCommands<FrebocapCommands>(TEXT("rebocap"), NSLOCTEXT("Contexts", "rebocap", "rebocap Plugin"), NAME_None, FrebocapStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};