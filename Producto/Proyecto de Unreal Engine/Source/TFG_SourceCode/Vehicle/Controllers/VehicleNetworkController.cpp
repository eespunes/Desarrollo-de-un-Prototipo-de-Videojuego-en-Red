// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "VehicleNetworkController.h"

#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AVehicleNetworkController::BeginPlay()
{
	Super::BeginPlay();
	vehiclePawn = Cast<AVehiclePawn>(GetPawn());
}

void AVehicleNetworkController::Accelerate()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Accelerate();
}

void AVehicleNetworkController::Brake()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Brake();
}

void AVehicleNetworkController::Steer(float value)
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Steer(value);
}

void AVehicleNetworkController::Drift()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Drift();
}

void AVehicleNetworkController::UseObject()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->UseObject();
}
