// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "FenrirRoarContainer.h"

#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AFenrirRoarContainer::UseObject()
{
	Super::UseObject();

	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 1;
	int32 vehicleInFrontPosition = currentPosition - 1;
	int32 vehicleBehindPosition = currentPosition + 1;

	URaceComponent* front = gameInstance->GetLevel()->GetVehicleByPosition(vehicleInFrontPosition);
	if (front)
		frontVehicle = Cast<AVehiclePawn>(front->GetOwner());
	URaceComponent* behind = gameInstance->GetLevel()->GetVehicleByPosition(vehicleBehindPosition);
	if (behind)
		behindVehicle = Cast<AVehiclePawn>(behind->GetOwner());

	if (frontVehicle)
	{
		frontVehicle->SetCurrentObject(fenrirRoar);
	}
	if (behindVehicle)
	{
		behindVehicle->SetCurrentObject(fenrirRoar);
	}

	SendObjectEvent();

	Stop();
}

void AFenrirRoarContainer::UseObject(FString frontUsername, FString behindUsername)
{
	Super::UseObject();

	URaceComponent* front = gameInstance->GetLevel()->GetVehicleByUsername(frontUsername);
	if (front)
		frontVehicle = Cast<AVehiclePawn>(front->GetOwner());
	URaceComponent* behind = gameInstance->GetLevel()->GetVehicleByUsername(behindUsername);
	if (behind)
		behindVehicle = Cast<AVehiclePawn>(behind->GetOwner());

	if (frontVehicle)
	{
		frontVehicle->SetCurrentObject(fenrirRoar);
	}
	if (behindVehicle)
	{
		behindVehicle->SetCurrentObject(fenrirRoar);
	}

	Stop();
}

void AFenrirRoarContainer::SetUp()
{
	objectType = 8;
	Super::SetUp();
}

void AFenrirRoarContainer::Stop()
{
	Super::Stop();

	Destroy();
}

void AFenrirRoarContainer::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FFenrirStruct* objectData = new FFenrirStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->used = used;

		if (frontVehicle)
		{
			objectData->objectiveUsername1 = frontVehicle->GetNetworkComponent()->GetUsername();
		}
		if (behindVehicle)
		{
			objectData->objectiveUsername2 = behindVehicle->GetNetworkComponent()->GetUsername();
		}

		FJsonObjectConverter::UStructToJsonObjectString<FFenrirStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}
