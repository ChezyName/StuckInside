// Copyright Epic Games, Inc. All Rights Reserved.

#include "StuckInsideGameMode.h"
#include "StuckInsideHUD.h"
#include "StuckInsideCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStuckInsideGameMode::AStuckInsideGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AStuckInsideHUD::StaticClass();
}
