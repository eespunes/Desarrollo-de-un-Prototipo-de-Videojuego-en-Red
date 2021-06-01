// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "NetworkComponent.h"

#include "DrawDebugHelpers.h"
#include "JsonObjectConverter.h"
#include "RaceComponent.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

// Sets default values for this component's properties
UNetworkComponent::UNetworkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UNetworkComponent::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	racingData = new FRacingDataStruct();
	if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (gameInstance)
		{
			if (gameInstance->IsMultiplayer())
			{
				raceComponent = Cast<AVehiclePawn>(GetOwner())->GetRaceComponent();

				raceID = gameInstance->GetRaceID();
				username = gameInstance->GetPlayer().pla_username;
				racingData->username = username;

				racingData->id = 0;

				gameInstance->SetLevel(
					Cast<ARacingLevel>(UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass())));
				isPlayer = true;

				// DELETE_delayTime = FMath::RandRange(0.00f, 0.1f);
				SendMessage();
			}
			else
			{
				ARacingLevel* level = Cast<ARacingLevel>(
					UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));
				if (level)
				{
					level->SetCanStartRace(true);
					username = TEXT("PLAYER 1");
				}
			}
		}
	}
	else
	{
	}
	Cast<AVehiclePawn>(GetOwner())->GetPlayerText()->SetText(username);
}

// Called every frame
void UNetworkComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if (isPlayer && gameInstance->GetLevel()->GetCanRace())
	// {
	// 	if (timer >= sendTime)
	// 	{
	// if (DELETE_delayTimer >= DELETE_delayTime)
	// {
	// 	DELETE_delayTimer = 0;
	// timer = 0;
	// SendMessage();
	// }
	// else
	// {
	// 	DELETE_delayTimer += DeltaTime;
	// }
	// }
	// else
	// {
	// 	timer += DeltaTime;
	// }
	// }
}

void UNetworkComponent::SendMessage()
{
	FVector location = GetOwner()->GetActorLocation();
	racingData->positionX = location.X;
	racingData->positionY = location.Y;
	racingData->positionZ = location.Z;


	FRotator rotation = GetOwner()->GetActorRotation();
	racingData->rotationRoll = rotation.Roll;
	racingData->rotationPitch = rotation.Pitch;
	racingData->rotationYaw = rotation.Yaw;

	racingData->id++;

	if (raceComponent)
	{
		racingData->time = raceComponent->GetTimeValue();
		racingData->lap = raceComponent->GetCurrentLap();
		racingData->checkpoint = raceComponent->GetExpectedCheckpoint();
		racingData->distance = raceComponent->GetDistance();
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
