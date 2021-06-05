// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "NetworkComponent.h"

#include "DrawDebugHelpers.h"
#include "JsonObjectConverter.h"
#include "RaceComponent.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

// Sets default values for this component's properties
UNetworkComponent::UNetworkComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UNetworkComponent::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	racingData = new FRacingDataStruct();
	vehicle = Cast<AVehiclePawn>(GetOwner());

	if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (gameInstance)
		{
			gameInstance->SetLevel(
				Cast<ARacingLevel>(UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass())));
			if (gameInstance->IsMultiplayer())
			{
				raceID = gameInstance->GetRaceID();
				username = gameInstance->GetPlayer().pla_username;
				racingData->username = username;

				racingData->id = 0;

				isPlayer = true;

				SendMessage();
			}
			else
			{
				if (gameInstance->GetLevel())
				{
					gameInstance->GetLevel()->SetCanStartRace(true);
				}
			}
		}
	}

	vehicle->GetPlayerText()->SetText(username);
}

void UNetworkComponent::SendMessage()
{
	FVector location = vehicle->GetOwner()->GetActorLocation();
	racingData->positionX = location.X;
	racingData->positionY = location.Y;
	racingData->positionZ = location.Z;

	UE_LOG(LogTemp, Warning,
		TEXT("%s Position= (%f,%f,%f)"), *username,
		location.X, location.Y,
		location.Z);


	FRotator rotation = vehicle->GetActorRotation();
	racingData->rotationRoll = rotation.Roll;
	racingData->rotationPitch = rotation.Pitch;
	racingData->rotationYaw = rotation.Yaw;

	racingData->id++;

	if (vehicle)
	{
		racingData->speed = vehicle->GetSpeed();

		racingData->driftValue = vehicle->GetDriftValue();

		racingData->accelerationTimer = vehicle->GetAccelerationTimer();
		racingData->brakingTimer = vehicle->GetBrakeTimer();
		racingData->decelerationTimer = vehicle->GetDecelerationTimer();
		racingData->driftIncreaseTimer = vehicle->GetDriftIncreaseTimer();
		racingData->driftDecreaseTimer = vehicle->GetDriftDecreaseTimer();

		racingData->time = vehicle->GetRaceComponent()->GetTimeValue();
		racingData->lap = vehicle->GetRaceComponent()->GetCurrentLap();
		racingData->checkpoint = vehicle->GetRaceComponent()->GetExpectedCheckpoint();
		racingData->distance = vehicle->GetRaceComponent()->GetDistance();
	}

	FString json;
	FJsonObjectConverter::UStructToJsonObjectString<FRacingDataStruct>(*racingData, json, 0, 0);

	FRacingStruct* racingStruct = new FRacingStruct();
	racingStruct->data = json;
	racingStruct->race = raceID;
	racingStruct->username = username;

	gameInstance->SendRaceEvents(*racingStruct);
}

void UNetworkComponent::SetUsername(FString other)
{
	username = other;
}

void UNetworkComponent::SetDataIsAccelerating()
{
	racingData->isAccelerating = !racingData->isAccelerating;
	if (gameInstance->GetLevel()->GetCanRace())
		SendMessage();
}

void UNetworkComponent::SetDataIsBraking()
{
	racingData->isBraking = !racingData->isBraking;
	if (gameInstance->GetLevel()->GetCanRace())
		SendMessage();
}

void UNetworkComponent::SetDataTurnValue(float value)
{
	racingData->turnValue = value;
	if (gameInstance->GetLevel()->GetCanRace())
		if (sendTimer > sendTurnTime)
		{
			// SendMessage();
			sendTimer = 0;
		}
		else
		{
			sendTimer += GetWorld()->DeltaTimeSeconds;
		}
}

void UNetworkComponent::SetDataIsDrifting()
{
	racingData->isDrifting = !racingData->isDrifting;
	if (gameInstance->GetLevel()->GetCanRace())
		SendMessage();
}

void UNetworkComponent::SetUseObjectData(bool use)
{
	racingData->useObject = use;
	SendMessage();
}

void UNetworkComponent::SetObjectData(int32 idx)
{
	racingData->object = idx;
	SendMessage();
}

bool UNetworkComponent::GetDataIsAccelerating() const
{
	return racingData->isAccelerating;
}

bool UNetworkComponent::GetDataIsBraking() const
{
	return racingData->isBraking;
}

bool UNetworkComponent::GetDataIsDrifting() const
{
	return racingData->isDrifting;
}

float UNetworkComponent::GetDataTurnValue() const
{
	return racingData->turnValue;
}

int32 UNetworkComponent::GetPing() const
{
	return ping;
}

void UNetworkComponent::SetPing(int32 Ping)
{
	ping = Ping;
}
