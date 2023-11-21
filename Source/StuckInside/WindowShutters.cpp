// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowShutters.h"

#include "Net/UnrealNetwork.h"

void AWindowShutters::Interact(ACharacter* Character)
{
	isOpened = !isOpened;
	GEngine->AddOnScreenDebugMessage(-1,1,FColor::Green,"Tryna Open / Close Window!");
	onDoorChanged();
	Super::Interact(Character);
}

AWindowShutters::AWindowShutters()
{
	Doors = CreateDefaultSubobject<USkeletalMeshComponent>("Doors");
	RootComponent = Doors;

	InteractionHB->Deactivate();
	InteractionHB->SetupAttachment(RootComponent);
}

void AWindowShutters::BeginPlay()
{
	DoorChangeEvent(isOpened);
	Super::BeginPlay();
}

void AWindowShutters::onDoorChanged_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1,1,FColor::Green,"Rep Notify -> Custom Event");
	DoorChangeEvent(isOpened);
}

void AWindowShutters::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AWindowShutters,isOpened);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
