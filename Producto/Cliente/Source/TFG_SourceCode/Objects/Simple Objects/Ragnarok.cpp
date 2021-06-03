// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Ragnarok.h"


#include "Components/DirectionalLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Objects/Containers/ObjectContainer.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void ARagnarok::UseObject()
{
	Super::UseObject();

	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ARacingLevel* level = gameInstance->GetLevel();
	level->SetCanStartRace(false);

	ADirectionalLight* directionalLight = Cast<ADirectionalLight>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (directionalLight)
	{
		directionalLight->GetComponent()->LightColor = FColor(255, 127, 0);
		directionalLight->GetComponent()->SetVisibility(false);
	}

	for (int i = 0; i < level->GetNumberOfVehicles(); ++i)
	{
		level->GetVehicle(i)->Ragnarok(i);
		Cast<AVehiclePawn>(level->GetVehicle(i)->GetOwner())->SetCurrentObject(nullptr);
	}

	TArray<AActor*> objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectContainer::StaticClass(), objects);
	for (auto object : objects)
	{
		object->Destroy();
	}
	vehicle->GetRaceComponent()->SetCanRace(false);
	if (directionalLight)
	{
		directionalLight->GetComponent()->SetVisibility(true);
	}
}

void ARagnarok::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (used)
	{
		// if (gameInstance->IsMultiplayer())
		// {
		// 	Stop();
		// 	Destroy();
		// }
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
