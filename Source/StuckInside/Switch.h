// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Switch.generated.h"

/**
 * 
 */
UCLASS()
class STUCKINSIDE_API ASwitch : public AInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess))
	UStaticMeshComponent* Switch;

	bool RightSide = true;
	bool rngSideIsRight = false;

	virtual void Interact(ACharacter* Character) override;

public:
	ASwitch();

	void BeginPlay() override;
	
	bool getSideActive()
	{
		return RightSide == rngSideIsRight;
	}

	void reset()
	{
		RightSide = (bool)(rand() % 2);
		rngSideIsRight = RightSide = (bool)(rand() % 2);
		if(Switch) Switch->SetRelativeRotation(FRotator(RightSide ? 0 : 180,0,0));
	}

	virtual FString getToolTip() override
	{
		return "Toggle Switch";
	}
};
