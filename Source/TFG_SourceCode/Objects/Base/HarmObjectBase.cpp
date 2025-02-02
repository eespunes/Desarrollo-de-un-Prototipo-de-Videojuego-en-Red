// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "HarmObjectBase.h"

#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AHarmObjectBase::SetUp()
{
	Super::SetUp();
	OnActorBeginOverlap.AddDynamic(this, &AHarmObjectBase::OnOverlapBegin);
}

void AHarmObjectBase::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
	if (vehiclePawn)
	{
		if (used && vehiclePawn != vehicle)
		{
			vehiclePawn->Damage();
			Destroy();
		}
	}
}
