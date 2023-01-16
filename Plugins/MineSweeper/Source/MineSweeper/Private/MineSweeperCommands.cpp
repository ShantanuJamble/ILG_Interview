// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeperCommands.h"

#define LOCTEXT_NAMESPACE "FMineSweeperModule"

void FMineSweeperCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MineSweeper", "Bring up MineSweeper window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
