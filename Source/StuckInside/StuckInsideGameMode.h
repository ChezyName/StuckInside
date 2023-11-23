// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BreakerDoor.h"
#include "Switch.h"
#include "WindowShutters.h"
#include "StuckInside/StuckInsideGS.h"
#include "GameFramework/GameModeBase.h"
#include "StuckInsideGameMode.generated.h"

UCLASS(minimalapi)
class AStuckInsideGameMode : public AGameModeBase
{
	GENERATED_BODY()

	float Breaker = 1.f;

public:
	AStuckInsideGameMode();

	UPROPERTY(EditAnywhere)
	float MaxPowerUsageTimeBeforeBreaker = 1.5f;

	UPROPERTY()
	AStuckInsideGS* SIGameState;
	
	UPROPERTY()
	TArray<AWindowShutters*> Windows;
	
	UPROPERTY()
	TArray<ASwitch*> BreakerSwitches;
	
	UPROPERTY()
	ABreakerDoor* BreakerDoor;

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;
};



