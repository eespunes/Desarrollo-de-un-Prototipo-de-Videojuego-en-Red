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

	directionalLight = Cast<ADirectionalLight>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (directionalLight)
	{
		directionalLight->GetComponent()->LightColor = FColor::Red;
		directionalLight->GetComponent()->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("All Red Everything"));
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
			directionalLight->GetComponent()->SetVisibility(true);
			Stop();
			Destroy();
		}
		else
		{
			timer += DeltaTime;
		}
	}
}
