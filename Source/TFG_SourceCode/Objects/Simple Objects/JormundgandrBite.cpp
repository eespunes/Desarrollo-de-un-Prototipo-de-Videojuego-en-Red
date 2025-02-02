// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "JormundgandrBite.h"


#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Objects/Base/ProtectObjectBase.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AJormundgandrBite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJormundgandrBite::UseObject()
{
	Super::UseObject();
	mesh->DestroyComponent();

	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 1;

	vehicles;
	for (int i = 0; i < attackPosition && i < currentPosition; i++)
	{
		vehicles.Add(Cast<AVehiclePawn>(gameInstance->GetLevel()->GetVehicleByPosition(i)->GetOwner()));
	}
	for (AVehiclePawn* otherVehicle : vehicles)
	{
		Bite(otherVehicle);
	}

	SendObjectEvent();

	Stop();
	Destroy();
}

void AJormundgandrBite::UseObject(TArray<FString> usernames)
{
	Super::UseObject();
	mesh->DestroyComponent();

	for (FString username : usernames)
	{
		URaceComponent* raceComponent = gameInstance->GetLevel()->GetVehicleByUsername(username);
		if (raceComponent)
		{
			AVehiclePawn* otherVehicle = Cast<AVehiclePawn>(raceComponent->GetOwner());

			Bite(otherVehicle);
		}
	}

	Stop();
	Destroy();
}

void AJormundgandrBite::Bite(AVehiclePawn* otherVehicle)
{
	otherVehicle->InstantiateParticle(jormundgandr);
	if (!Cast<AProtectObjectBase>(otherVehicle->GetCurrentObject()))
		otherVehicle->Damage();
}

void AJormundgandrBite::SetUp()
{
	objectType = 7;
	Super::SetUp();
}

void AJormundgandrBite::Stop()
{
	Super::Stop();
	Destroy();
}

void AJormundgandrBite::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FJormundgandrStruct* objectData = new FJormundgandrStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->used = used;

		if (vehicles.Num() >= 1)
		{
			objectData->objectiveUsername1 = vehicles[0]->GetNetworkComponent()->GetUsername();

			if (vehicles.Num() >= 2)
			{
				objectData->objectiveUsername2 = vehicles[1]->GetNetworkComponent()->GetUsername();
			}
			if (vehicles.Num() == 3)
			{
				objectData->objectiveUsername3 = vehicles[2]->GetNetworkComponent()->GetUsername();
			}
		}

		FJsonObjectConverter::UStructToJsonObjectString<FJormundgandrStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}
