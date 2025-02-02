// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "FenrirRoar.h"

#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AFenrirRoar::SetUp()
{
	objectType = 10;
	Super::SetUp();
	OnActorBeginOverlap.AddDynamic(this, &AFenrirRoar::OnOverlapBegin);
}

void AFenrirRoar::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	if (used)
	{
		AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
		if (vehiclePawn)
		{
			if (vehiclePawn != vehicle)
			{
				vehiclePawn->SetCurrentObject(fenrirRoar);
			}
		}
	}
}

void AFenrirRoar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		if (timer >= boostDuration)
			Stop();
		timer += DeltaTime;
	}
	else
	{
		if (vehicle)
			UseObject();
	}
}

void AFenrirRoar::UseObject()
{
	Super::UseObject();
	vehicle->SetMaxSpeedMultiplier(boostIncrease);
}

void AFenrirRoar::Stop()
{
	Super::Stop();
	vehicle->ResetMaxSpeedMultiplier();
	Destroy();
}
