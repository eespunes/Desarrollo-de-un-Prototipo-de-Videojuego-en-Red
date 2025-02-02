// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "LokiTrick.h"


#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void ALokiTrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		if (timer >= duration)
			Stop();
		timer += DeltaTime;
	}
}

void ALokiTrick::UseObject()
{
	Super::UseObject();

	position = this->vehicle->GetRaceComponent()->GetPosition() - 2;
	SendObjectEvent();
	for (int i = position; i >= 0; i--)
	{
		AVehiclePawn* otherVehicle = Cast<AVehiclePawn>(gameInstance->GetLevel()->GetVehicleByPosition(i)->GetOwner());
		if (otherVehicle)
		{
			otherVehicle->InstantiateParticle(particle);
			otherVehicle->InvertControls();
			tricked.Add(otherVehicle);
		}
	}
}

void ALokiTrick::UseObject(int32 desiredPosition)
{
	Super::UseObject();

	for (int i = desiredPosition; i >= 0; i--)
	{
		AVehiclePawn* otherVehicle = Cast<AVehiclePawn>(gameInstance->GetLevel()->GetVehicleByPosition(i)->GetOwner());
		if (otherVehicle)
		{
			otherVehicle->InstantiateParticle(particle);
			otherVehicle->InvertControls();
			tricked.Add(otherVehicle);
		}
	}
}

void ALokiTrick::SetUp()
{
	objectType = 5;
	Super::SetUp();
}

void ALokiTrick::Stop()
{
	timer = 0;
	for (AVehiclePawn* otherVehicle : tricked)
	{
		otherVehicle->NormalControls();
	}

	Super::Stop();
	Destroy();
}

void ALokiTrick::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FLokiStruct* objectData = new FLokiStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->position = position;
		objectData->used = used;
		FJsonObjectConverter::UStructToJsonObjectString<FLokiStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}
