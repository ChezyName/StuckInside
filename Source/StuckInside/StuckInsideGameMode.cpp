// Copyright Epic Games, Inc. All Rights Reserved.

#include "StuckInsideGameMode.h"

#include "EngineUtils.h"
#include "StuckInsideHUD.h"
#include "StuckInsideCharacter.h"
#include "StuckInsideGS.h"
#include "GameFramework/PlayerState.h"
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

void AStuckInsideGameMode::onAllPlayersReady_Implementation()
{
	if(GameStarted) return;
	GameStarted = true;

	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green, "Game Is Starting Real Soon [C++]");

	//PlayersPick Spawn
	AStuckInsideGS* GS = Cast<AStuckInsideGS>(GameState);
	if(GS)
	{
		for (APlayerController* Controller : TActorRange<APlayerController>(GetWorld()))
		{
			GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green, "Summoning " + Controller->GetName());
			//code
			if(Controller->GetPlayerState<APlayerState>() == GS->DemonPlayer)
			{
				//Spawn DEMON
				FActorSpawnParameters PlayerSpawnParameters{};
				APawn* NewChar = GetWorld()->SpawnActor<APawn>(Demon, FindPlayerStart(Controller,"Demon") ? FindPlayerStart(Controller,"Demon")->GetActorLocation() : FVector::ZeroVector, FRotator::ZeroRotator, PlayerSpawnParameters);
				if(Controller->GetPawn()) Controller->GetPawn()->Destroy();
				Controller->Possess(NewChar);
			}
			else
			{
				//Spawn HUMAN
				FActorSpawnParameters PlayerSpawnParameters{};
				APawn* NewChar = GetWorld()->SpawnActor<APawn>(Survivors, FindPlayerStart(Controller,"Human") ? FindPlayerStart(Controller,"Human")->GetActorLocation() : FVector::ZeroVector, FRotator::ZeroRotator, PlayerSpawnParameters);
				if(Controller->GetPawn()) Controller->GetPawn()->Destroy();
				Controller->Possess(NewChar);
			}
		}
	}
}

void AStuckInsideGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("Spawning New player"));
	FVector StartLocation =  ChoosePlayerStart(NewPlayer) ? ChoosePlayerStart(NewPlayer)->GetActorLocation() : FVector::ZeroVector;
	if(GameStarted)
	{
		FActorSpawnParameters PlayerSpawnParameters{};
		APawn* NewChar = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, StartLocation, FRotator::ZeroRotator, PlayerSpawnParameters);
		NewPlayer->Possess(NewChar);
	}
	else
	{
		FActorSpawnParameters PlayerSpawnParameters{};
		APawn* NewChar = GetWorld()->SpawnActor<APawn>(LobbyClass, StartLocation, FRotator::ZeroRotator, PlayerSpawnParameters);
		NewPlayer->Possess(NewChar);
	}
}

void AStuckInsideGameMode::BeginPlay()
{
	SIGameState = GetGameState<AStuckInsideGS>();
	//SetActorTickInterval(TickTime);
	if(GetWorld())
	{
		TArray<AActor*> FoundActorsA;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWindowShutters::StaticClass(), FoundActorsA);
		for(AActor* Actor : FoundActorsA)
		{
			AWindowShutters* w = Cast<AWindowShutters>(Actor);
			if(w)
			{
				Windows.Add(w);
			}
		}

		TArray<AActor*> FoundActorsB;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASwitch::StaticClass(), FoundActorsB);
		for(AActor* Actor : FoundActorsB)
		{
			ASwitch* s = Cast<ASwitch>(Actor);
			if(s)
			{
				BreakerSwitches.Add(s);
			}
		}

		BreakerDoor = Cast<ABreakerDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABreakerDoor::StaticClass()));
	}
	Super::BeginPlay();
}

void AStuckInsideGameMode::KillPlayer(AStuckInsideCharacter* Character)
{
}

void AStuckInsideGameMode::Tick(float DeltaSeconds)
{
	int WindowsActive = 0;
	for(AWindowShutters* Window : Windows)
	{
		if(!Window->getIsOpened()) WindowsActive++;
	}

	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,"Power Usage Time: " + FString::SanitizeFloat(Breaker));
	GEngine->AddOnScreenDebugMessage(-1,0,SIGameState->PowerActive ? FColor::Green : FColor::Red,"POWER");

	WindowsActive = FMath::Clamp(WindowsActive,0,3);
	if(SIGameState) SIGameState->PowerUsage = WindowsActive;

	if(WindowsActive >= 3)
	{
		GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,"Power Depleting: " + FString::SanitizeFloat(Breaker));
		Breaker -= DeltaSeconds;
		if(SIGameState->PowerActive && Breaker <= 0)
		{
			//Turn Off Power
			SIGameState->PowerActive = false;

			for(AWindowShutters* Window : Windows)
			{
				Window->Close();
			}

			for(ASwitch* s : BreakerSwitches) s->reset();
			BreakerDoor->Close();
		}
	}
	else
	{
		Breaker += DeltaSeconds;
	}

	//Power Checking
	bool SwitchIsOn = true;
	for(ASwitch* s : BreakerSwitches) if(!s->getSideActive()) SwitchIsOn = false;
	if(SwitchIsOn)
	{
		SIGameState->PowerActive = true;
	}
	else
	{
		//Turn Off Power
		SIGameState->PowerActive = false;
		Breaker = 0;

		for(AWindowShutters* Window : Windows)
		{
			Window->Close();
		}
	}

	Breaker = FMath::Clamp(Breaker,0.f,MaxPowerUsageTimeBeforeBreaker);
	Super::Tick(DeltaSeconds);
}
