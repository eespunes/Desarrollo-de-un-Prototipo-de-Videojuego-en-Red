// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Ragnarok.h"


#include "JsonObjectConverter.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Objects/Containers/ObjectContainer.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void ARagnarok::UseObject()
{
	Super::UseObject();

	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	gameInstance->GetLevel()->SetCanStartRace(false);
	
	SendObjectEvent();

	for (int i = 0; i < gameInstance->GetLevel()->GetNumberOfVehicles(); ++i)
	{
		gameInstance->GetLevel()->GetVehicleByPosition(i)->Ragnarok(i);
		Cast<AVehiclePawn>(gameInstance->GetLevel()->GetVehicleByPosition(i)->GetOwner())->SetCurrentObject(nullptr);
	}

	TArray<AActor*> objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectContainer::StaticClass(), objects);
	for (auto object : objects)
	{
		object->Destroy();
	}
	vehicle->GetRaceComponent()->SetCanRace(false);
}

void ARagnarok::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (used)
	{
		if (gameInstance->IsMultiplayer())
		{
			Stop();
			Destroy();
		}
		if (timer > time)
		{
			gameInstance->GetLevel()->SetCanStartRace(true);
			Stop();
			Destroy();
		}
		else
		{
			timer += DeltaTime;
		}
	}
}

void ARagnarok::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FRagnarokStruct* objectData = new FRagnarokStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->used = used;
		objectData->ragnarok = true;
		FJsonObjectConverter::UStructToJsonObjectString<FRagnarokStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}

void ARagnarok::SetUp()
{
	objectType = 9;
	Super::SetUp();
}
