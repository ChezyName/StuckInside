// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StuckInsideGS.generated.h"

/**
 * 
 */
UCLASS()
class STUCKINSIDE_API AStuckInsideGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,Replicated)
	int PowerUsage = 0;

	UPROPERTY(BlueprintReadOnly,Replicated)
	bool PowerActive = true;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
