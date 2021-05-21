// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "NetworkComponent.h"

#include "DrawDebugHelpers.h"
#include "JsonObjectConverter.h"
#include "Chaos/ChaosPerfTest.h"
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
	racingData = new FRacingDataStruct();
	if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
		if (gameInstance)
		{
			if (gameInstance->IsMultiplayer())
			{
				raceID = gameInstance->GetRaceID();
				username = gameInstance->GetPlayer().pla_username;
				racingData->username = username;
				gameInstance->SetLevel(
					Cast<ARacingLevel>(UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass())));
				isPlayer = true;
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

// Called every frame
void UNetworkComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isPlayer && gameInstance->GetLevel()->GetCanRace())
	{
		if (timer >= sendTime)
		{
			timer = 0;
			messageId++;
			SendMessage();
		}
		else
		{
			timer += DeltaTime;
		}
	}
	DrawDebugString(GEngine->GetWorldFromContextObject(this),
	                GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 100,
	                *FString::Printf(TEXT("%s"), *username), NULL, FColor::Black, DeltaTime, false);
}

void UNetworkComponent::SendMessage()
{
	FVector location = GetOwner()->GetActorLocation();
	racingData->positionX = location.X;
	racingData->positionY = location.Y;
	racingData->positionZ = location.Z;

	
	FRotator rotation = GetOwner()->GetActorRotation();
	racingData->rotationRoll=rotation.Roll;
	racingData->rotationPitch=rotation.Pitch;
	racingData->rotationYaw=rotation.Yaw;

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

void UNetworkComponent::SetDataIsAccelerating(bool accelerating)
{
	racingData->isAccelerating = accelerating;
}

void UNetworkComponent::SetDataIsBraking(bool braking)
{
	racingData->isBraking = braking;
}

void UNetworkComponent::SetDataTurnValue(float value)
{
	racingData->turnValue = value;
}

void UNetworkComponent::SetDataIsDrifting(bool drifting)
{
	racingData->isDrifting = drifting;
}
void UNetworkComponent::SetUseObjectData(bool use)
{
	racingData->useObject = use;
}

void UNetworkComponent::SetObjectData(int32 idx)
{
	racingData->object = idx;
}
