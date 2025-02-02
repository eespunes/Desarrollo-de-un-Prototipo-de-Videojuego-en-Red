// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "OdinEye.h"

#include <algorithm>


#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AOdinEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOdinEye::UseObject()
{
	Super::UseObject();

	FVector position;
	FRotator rotation;

	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 2;
	if (currentPosition < 0)
	{
		checkPoint = vehicle->GetRaceComponent()->GetCurrentCheckPoint();
	}
	else
	{
		int32 expectedCheckpoint = gameInstance->GetLevel()->GetVehicleByPosition(currentPosition)->
		                                         GetCheckpointPosition();
		int32 leaderExpectedCheckpoint = gameInstance->GetLevel()->GetVehicleByPosition(0)->GetCheckpointPosition();

		if (expectedCheckpoint == leaderExpectedCheckpoint)
		{
			expectedCheckpoint = vehicle->GetRaceComponent()->GetCheckpointPosition();
		}

		int32 difference = leaderExpectedCheckpoint - expectedCheckpoint;
		difference *= FMath::RandRange(min, max);
		difference = difference == 0 ? 1 : difference;
		expectedCheckpoint += difference;

		checkPoint = gameInstance->GetLevel()->GetCheckpoint(expectedCheckpoint);
	}
	SendObjectEvent();
	Teleport();
}

void AOdinEye::UseObject(int checkpointPosition)
{
	Super::UseObject();

	checkPoint = gameInstance->GetLevel()->GetCheckpoint(checkpointPosition);

	Teleport();
}

void AOdinEye::Teleport()
{
	gameInstance->Teleport(checkPoint,vehicle,offset);
	vehicle->InstantiateParticle(particle);

	Stop();
	Destroy();
}

void AOdinEye::SetUp()
{
	objectType = 6;
	Super::SetUp();
}

void AOdinEye::Stop()
{
	Super::Stop();
	Destroy();
}

void AOdinEye::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FOdinStruct* objectData = new FOdinStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->destination = checkPoint->GetPosition();
		objectData->used = used;
		FJsonObjectConverter::UStructToJsonObjectString<FOdinStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}
