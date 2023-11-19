// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "WindowShutters.generated.h"

/**
 * 
 */
UCLASS()
class STUCKINSIDE_API AWindowShutters : public AInteractable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess),BlueprintReadOnly)
	USkeletalMeshComponent* Doors;
	
	UPROPERTY(Replicated,ReplicatedUsing=onDoorChanged)
	bool isOpened = false;

	virtual void Interact(ACharacter* Character) override;

	UFUNCTION(NetMulticast,Reliable)
	void onDoorChanged();

public:
	AWindowShutters();

	UFUNCTION(BlueprintImplementableEvent)
	void DoorChangeEvent(bool isOpen);

	UFUNCTION(BlueprintGetter)
	bool getIsOpened(){ return isOpened; }

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
