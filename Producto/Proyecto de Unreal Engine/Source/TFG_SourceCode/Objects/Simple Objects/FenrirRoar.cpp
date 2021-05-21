// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "FenrirRoar.h"

#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AFenrirRoar::SetUp()
{
	Super::SetUp();
	OnActorBeginOverlap.AddDynamic(this, &AFenrirRoar::OnOverlapBegin);
}

void AFenrirRoar::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	if(used)
	{
		AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
		if (vehiclePawn)
		{
			if (vehiclePawn != vehicle)
			{
				if (vehiclePawn->GetMaxSpeed() != vehiclePawn->GetInitialMaxSpeed() * boostIncrease)
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
	vehicle->SetMaxSpeed(vehicle->GetInitialMaxSpeed() * boostIncrease);
}

void AFenrirRoar::Stop()
{
	Super::Stop();
	vehicle->SetMaxSpeed(vehicle->GetInitialMaxSpeed());
	Destroy();
}
