// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakerDoor.h"

void ABreakerDoor::Interact(ACharacter* Character)
{
	Super::Interact(Character);

	if(isOpened)
	{
		//BreakerDoor->SetRelativeRotation(FRotator(0,CloseRotationYaw,0));
		isOpened = false;
	}
	else
	{
		//BreakerDoor->SetRelativeRotation(FRotator(0,OpenRotationYaw,0));
		isOpened = true;
	}
}

void ABreakerDoor::Close()
{
	if(HasAuthority())
	{
		//BreakerDoor->SetRelativeRotation(FRotator(0,CloseRotationYaw,0));
		isOpened = false;
	}
}

void ABreakerDoor::Tick(float DeltaSeconds)
{
	if(HasAuthority())
	{
		float RotationYaw = isOpened ? OpenRotationYaw : CloseRotationYaw;
		BreakerDoor->SetRelativeRotation(
			FRotator(0,
				FMath::Lerp(BreakerDoor->GetRelativeRotation().Yaw,RotationYaw,OpenSpeed * DeltaSeconds)
			,0)
		);
	}
	Super::Tick(DeltaSeconds);
}

ABreakerDoor::ABreakerDoor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	BreakerDoor = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	BreakerDoor->SetupAttachment(RootComponent);
	BreakerDoor->SetIsReplicated(true);
	InteractionHB->SetupAttachment(BreakerDoor);
}
