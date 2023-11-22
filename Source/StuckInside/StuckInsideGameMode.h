// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WindowShutters.h"
#include "StuckInside/StuckInsideGS.h"
#include "GameFramework/GameModeBase.h"
#include "StuckInsideGameMode.generated.h"

UCLASS(minimalapi)
class AStuckInsideGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStuckInsideGameMode();

	UPROPERTY(EditAnywhere)
	float TickTime = 1.5f;

	UPROPERTY()
	AStuckInsideGS* SIGameState;
	UPROPERTY()
	TArray<AWindowShutters*> Windows;

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;
};



