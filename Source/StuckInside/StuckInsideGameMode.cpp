// Copyright Epic Games, Inc. All Rights Reserved.

#include "StuckInsideGameMode.h"
#include "StuckInsideHUD.h"
#include "StuckInsideCharacter.h"
#include "StuckInsideGS.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AStuckInsideGameMode::AStuckInsideGameMode()
	: Super()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AStuckInsideHUD::StaticClass();
}

void AStuckInsideGameMode::BeginPlay()
{
	SIGameState = GetGameState<AStuckInsideGS>();
	SetActorTickInterval(TickTime);
	if(GetWorld())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWindowShutters::StaticClass(), FoundActors);
		for(AActor* Actor : FoundActors)
		{
			AWindowShutters* w = Cast<AWindowShutters>(Actor);
			if(w)
			{
				Windows.Add(w);
			}
		}
	}
	Super::BeginPlay();
}

void AStuckInsideGameMode::Tick(float DeltaSeconds)
{
	int WindowsActive = 0;
	for(AWindowShutters* Window : Windows)
	{
		if(!Window->getIsOpened()) WindowsActive++;
	}

	//GEngine->AddOnScreenDebugMessage(-1,1,FColor::Green,FString::SanitizeFloat(WindowsActive) + " Windows Are Closed!");

	WindowsActive = FMath::Clamp(WindowsActive,0,3);
	if(SIGameState) SIGameState->PowerUsage = WindowsActive;

	Super::Tick(DeltaSeconds);
}
