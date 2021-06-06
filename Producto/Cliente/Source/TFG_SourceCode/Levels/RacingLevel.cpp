// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingLevel.h"

#include "Kismet/GameplayStatics.h"
#include "Serialization/JsonTypes.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/RaceControllers/RaceController.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

class AVehiclePawn;

void ARacingLevel::BeginPlay()
{
	raceController = GetWorld()->SpawnActor<ARaceController>(ARaceController::StaticClass(), GetActorLocation(),
	                                                         GetActorRotation());
}

ARacingLevel::ARacingLevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

int32 ARacingLevel::GetNumberOfLaps() const
{
	return numberOfLaps;
}

int32 ARacingLevel::GetNumberOfCheckpoints() const
{
	if (!raceController)
		return -1;
	return raceController->GetNumberOfCheckpoints();
}

int32 ARacingLevel::GetNumberOfVehicles() const
{
	if (!raceController)
		return -1;
	return raceController->GetNumberOfVehicles();
}

ACheckPoint* ARacingLevel::GetCheckpoint(int32 idx) const
{
	if (!raceController)
		return nullptr;
	return raceController->GetCheckpoint(idx);
}

inline URaceComponent* ARacingLevel::GetVehicle(int32 position)
{
	if (!raceController)
		return nullptr;
	return raceController->GetVehicle(position);
}

bool ARacingLevel::GetCanRace() const
{
	if (!raceController)
		return nullptr;
	return raceController->GetCanRace();
}

void ARacingLevel::SetCanRace(bool value) const
{
	if (!raceController)
		return;
	raceController->SetCanRace(value);
}

bool ARacingLevel::GetCanStartRace() const
{
	if (!raceController)
		return nullptr;
	return raceController->GetCanStartRace();
}

void ARacingLevel::SetCanStartRace(bool value) const
{
	if (!raceController)
		return;
	raceController->SetCanStartRace(value);
}

APlayerStart* ARacingLevel::GetRaceStart(int idx) const
{
	if (!raceController)
		return nullptr;
	return raceController->GetRaceStart(idx);
}

void ARacingLevel::SetPlayerHasFinished()
{
	playerHasFinished = true;
}

bool ARacingLevel::GetPlayerHasFinished()
{
	return playerHasFinished;
}

TArray<URaceComponent*> ARacingLevel::GetVehicles() const
{
	if (!raceController)
	{
		TArray<URaceComponent*> foundActors;
		return foundActors;
	}
	return raceController->GetVehicles();
}

void ARacingLevel::SetClassification(TArray<URaceComponent*> classification)
{
	if (!raceController)
	{
		return;
	}
	return raceController->SetClassification(classification);
}
