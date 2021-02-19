// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Ragnarok.h"


#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void ARagnarok::UseObject()
{
	Super::UseObject();

	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	for (int i = 0; i < level->GetNumberOfVehicles(); ++i)
	{
		level->GetVehicle(i)->Ragnarok(i);
	}

	vehicle->GetRaceComponent()->SetCanRace(false);

	Stop();
	Destroy();
}
