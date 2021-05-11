// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "NetworkComponent.h"

#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

// Sets default values for this component's properties
UNetworkComponent::UNetworkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNetworkComponent::BeginPlay()
{
	Super::BeginPlay();

	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInstance)
	{
		if (gameInstance->IsMultiplayer())
		{
			raceID = gameInstance->GetRaceID();
			username = gameInstance->GetPlayer().pla_username;

			FRacingStruct* racingStruct = new FRacingStruct();
			racingStruct->message = TEXT("Hello from the other side");
			racingStruct->race = raceID;
			racingStruct->username = username;
			gameInstance->SendRaceEvents(*racingStruct);
		}
	}
}

// Called every frame
void UNetworkComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
