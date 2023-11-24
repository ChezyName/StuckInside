// Fill out your copyright notice in the Description page of Project Settings.


#include "Demon.h"

#include "Door.h"
#include "StuckInsideCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADemon::ADemon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetIsReplicated(true);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	GlobalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	GlobalMesh->SetOnlyOwnerSee(true);
	GlobalMesh->SetupAttachment(RootComponent);
	GlobalMesh->SetOwnerNoSee(true);

	BiteHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("BiteHitbox"));
	BiteHitbox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADemon::BeginPlay()
{
	if(HasAuthority()) SpawnLoc = GetActorLocation();
	Super::BeginPlay();
	
}

void ADemon::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADemon::Bite);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ADemon::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADemon::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADemon::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADemon::LookUpAtRate);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called every frame
void ADemon::Tick(float DeltaTime)
{
	if(HasAuthority()) BiteCD -= DeltaTime;
	Super::Tick(DeltaTime);

}

void ADemon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ADemon,BiteCD);
	DOREPLIFETIME(ADemon,BiteCount);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ADemon::BiteAnimationPlayAll_Implementation()
{
	PlayAnimMontage(BiteAnimation);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),BiteSFX,GetActorLocation());
}

void ADemon::Bite_Implementation()
{
	if(BiteCD <= 0 && BiteCount > 0)
	{
		BiteCount -= 1;
		BiteCD = BiteCooldownPerBite;
		BiteAnimationPlayAll();
		
		//Bite With Animation Playing
		TArray<AActor*> ActorsHit;
		BiteHitbox->GetOverlappingActors(ActorsHit);

		for(AActor* Actor : ActorsHit)
		{
			ADoor* DoorObject = Cast<ADoor>(Actor);
			AStuckInsideCharacter* HumanObject = Cast<AStuckInsideCharacter>(Actor);

			if(DoorObject)
			{
				UStaticMeshComponent* DoorMesh = DoorObject->GetDoor();
				DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				DoorMesh->SetVisibility(false);
				break;
			}

			if(HumanObject)
			{
				//Kill Human

				//Disapear Back To Spawn
				SetActorLocation(SpawnLoc);
				break;
			}
		}

		if(BiteCount <= 0) SetActorLocation(SpawnLoc);
	}
}


void ADemon::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ADemon::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ADemon::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADemon::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}