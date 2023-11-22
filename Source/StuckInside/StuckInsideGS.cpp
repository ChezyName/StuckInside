// Fill out your copyright notice in the Description page of Project Settings.


#include "StuckInsideGS.h"

#include "Net/UnrealNetwork.h"

void AStuckInsideGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AStuckInsideGS,PowerUsage);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
