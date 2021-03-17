// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Ragnarok.h"


#include "Components/DirectionalLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void ARagnarok::UseObject()
{
	Super::UseObject();

	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	for (int i = 0; i < level->GetNumberOfVehicles(); ++i)
	{
		level->GetVehicle(i)->Ragnarok(i);
		Cast<AVehiclePawn>(level->GetVehicle(i)->GetOwner())->SetCurrentObject(nullptr);
	}

	USkyAtmosphereComponent* skyAtmosphere = Cast<USkyAtmosphereComponent>(
		UGameplayStatics::GetActorOfClass(GetWorld(), USkyAtmosphereComponent::StaticClass()));
	UE_LOG(LogTemp, Warning, TEXT("Yeppaa"));
	if (skyAtmosphere)
	{
		skyAtmosphere->RayleighScattering = FLinearColor::Red;
		UE_LOG(LogTemp, Error, TEXT("All Red Everything"));
	}

	vehicle->GetRaceComponent()->SetCanRace(false);

	Stop();
	Destroy();
}
