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
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UNetworkComponent::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	racingData = new FRacingDataStruct();
	vehicle = Cast<AVehiclePawn>(GetOwner());

	canSend = GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn();
	if (canSend)
	{
		if (gameInstance)
		{
			if (!gameInstance->GetLevel())
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
	if (!vehicle->GetRaceComponent()->HasFinished() || !notSendedFinish)
	{
		FVector currentLocation = vehicle->GetActorLocation();
		racingData->positionX = currentLocation.X;
		racingData->positionY = currentLocation.Y;
		racingData->positionZ = currentLocation.Z;

		FRotator rotation = vehicle->GetActorRotation();
		racingData->rotationRoll = rotation.Roll;
		racingData->rotationPitch = rotation.Pitch;
		racingData->rotationYaw = rotation.Yaw;

		racingData->id++;
		racingData->finished = vehicle->GetRaceComponent()->HasFinished();
		if (vehicle->GetRaceComponent()->HasFinished())
		{
			notSendedFinish = true;
		}

		if (vehicle)
		{
			racingData->speed = vehicle->GetSpeed();

			racingData->driftValue = vehicle->GetDriftValue();

			racingData->accelerationTimer = vehicle->GetAccelerationTimer();
			racingData->brakingTimer = vehicle->GetBrakeTimer();
			racingData->decelerationTimer = vehicle->GetDecelerationTimer();
			racingData->driftIncreaseTimer = vehicle->GetDriftIncreaseTimer();
			racingData->driftDecreaseTimer = vehicle->GetDriftDecreaseTimer();

			racingData->hit = vehicle->GetHasBeenHit();

			racingData->time = vehicle->GetRaceComponent()->GetTimeValue();
			racingData->lap = vehicle->GetRaceComponent()->GetCurrentLap();
			racingData->checkpoint = vehicle->GetRaceComponent()->GetCheckpointPosition();
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
}

void UNetworkComponent::SetUsername(FString other)
{
	username = other;
}

void UNetworkComponent::SetDataIsAccelerating()
{
	racingData->isAccelerating = !racingData->isAccelerating;
	SendMessage();
}

void UNetworkComponent::SetDataIsBraking()
{
	racingData->isBraking = !racingData->isBraking;
	SendMessage();
}

void UNetworkComponent::SetDataTurnValue(float value)
{
	racingData->turnValue = value;
	if (gameInstance->GetLevel()->GetCanRace())
		if (sendTimer > sendTurnTime)
		{
			SendMessage();
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

FString UNetworkComponent::GetUsername()
{
	return username;
}
