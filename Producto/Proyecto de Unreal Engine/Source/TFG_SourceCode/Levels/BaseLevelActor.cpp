// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLevelActor.h"

#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

class AVehiclePawn;

void ABaseLevelActor::BeginPlay()
{
	FindCheckpoints();

	FindVehicles();
}

ABaseLevelActor::ABaseLevelActor()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABaseLevelActor::FindCheckpoints()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		ACheckPoint* checkpoint = Cast<ACheckPoint>(actor);
		if (checkpoint)
		{
			checkpoints.Add(checkpoint);
		}
	}
	numberOfCheckpoints = checkpoints.Num();
}

void ABaseLevelActor::FindVehicles()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehiclePawn::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		AVehiclePawn* vehicle = Cast<AVehiclePawn>(actor);
		if (vehicle)
		{
			vehicles.Add(vehicle->GetRaceComponent());
		}
	}
}

int ABaseLevelActor::GetNumberOfLaps() const
{
	return numberOfLaps;
}

int ABaseLevelActor::GetNumberOfCheckpoints() const
{
	return numberOfCheckpoints;
}

ACheckPoint* ABaseLevelActor::GetCheckpoint(int32 idx)
{
	if (idx < checkpoints.Num())
		return checkpoints[idx];
	return nullptr;
}

void ABaseLevelActor::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	for (URaceComponent* raceComponent : vehicles)
	{
		raceComponent->CalculateDistance(checkpoints[raceComponent->GetExpectedCheckpoint()]);
	}
	for (int i = 0; i < vehicles.Num(); i++)
	{
		vehicles[i]->SetPosition(i + 1);
	}
}
