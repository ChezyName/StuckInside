// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Demon.generated.h"

UCLASS()
class STUCKINSIDE_API ADemon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADemon();

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* GlobalMesh;

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	UBoxComponent* BiteHitbox;

	UPROPERTY()
	FVector SpawnLoc;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MAIN, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MAIN, meta = (AllowPrivateAccess = "true"))
	float BiteCooldownPerBite = 0.75f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MAIN, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BiteAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MAIN, meta = (AllowPrivateAccess = "true"))
	USoundWave* BiteSFX;
	
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);
	
	void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server,Reliable)
	void Bite();

	UFUNCTION(NetMulticast,Reliable)
	void BiteAnimationPlayAll();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated,BlueprintReadOnly)
	float BiteCD = 0;

	UPROPERTY(Replicated,BlueprintReadOnly)
	int BiteCount = 3;

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh() const { return GlobalMesh; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
