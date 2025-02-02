// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Sleipnir.h"

#include "Math/UnitConversion.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void ASleipnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		if (timer >= boostDuration)
			Stop();
		timer += DeltaTime;
	}
}

void ASleipnir::UseObject()
{
	Super::UseObject();
	SendObjectEvent();
	vehicle->SetMaxSpeedMultiplier(boostIncrease);
}

void ASleipnir::SetUp()
{
	objectType=2;
	Super::SetUp();
}

void ASleipnir::Stop()
{
	Super::Stop();
	vehicle->ResetMaxSpeedMultiplier();
	Destroy();
}
