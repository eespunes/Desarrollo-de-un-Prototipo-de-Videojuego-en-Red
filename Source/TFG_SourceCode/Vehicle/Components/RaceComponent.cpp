// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "RaceComponent.h"


#include <iterator>
#include <string>


#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"

// Sets default values for this component's properties
URaceComponent::URaceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URaceComponent::BeginPlay()
{
	Super::BeginPlay();

	vehicle = Cast<AVehiclePawn>(GetOwner());
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!gameInstance->GetLevel())
	{
		gameInstance->SetLevel(
			Cast<ARacingLevel>(UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass())));
	}

	maxCheckpoints = gameInstance->GetLevel()->GetNumberOfCheckpoints();
	maxLaps = gameInstance->GetLevel()->GetNumberOfLaps();
	checkpoint = gameInstance->GetLevel()->GetCheckpoint(0);
	isPlayer = GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void URaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CanRace() && !finished)
	{
		currentLapTime += DeltaTime;
		if (isPlayer)
			generalLapTime += DeltaTime;

		if (checkpoint->IsPlayerInPlane(GetOwner()->GetActorLocation()))
		{
			SetCheckpoint();
		}
	}
}

void URaceComponent::SetCheckpoint()
{
	if (checkpoint->GetPosition() == (maxCheckpoints - 1))
	{
		currentLap++;
		if (bestLapTime == -1 || currentLapTime <= bestLapTime)
			bestLapTime = currentLapTime;
		currentLapTime = 0;
		if (gameInstance->IsMultiplayer() && currentLap == maxLaps)
		{
			GetOwner()->SetHidden(true);
			finished = true;
			GetOwner()->SetActorLocation(FVector(0, 0, -10000));
			if (isPlayer)
			{
				gameInstance->GetLevel()->SetPlayerHasFinished();
				vehicle->GetNetworkComponent()->SendMessage();
			}
		}
	}
	
	checkpoint = gameInstance->GetLevel()->
	                           GetCheckpoint((checkpoint->GetPosition() + 1) % maxCheckpoints);
}

int URaceComponent::GetPosition() const
{
	return position;
}

ACheckPoint* URaceComponent::GetCurrentCheckPoint() const
{
	return checkpoint;
}

void URaceComponent::SetPosition(int32 Position)
{
	position = Position;
}

int32 URaceComponent::GetCurrentLap() const
{
	return currentLap;
}

int32 URaceComponent::GetMaxLap() const
{
	return maxLaps;
}

float URaceComponent::GetDistance()
{
	FVector checkpointLocation = checkpoint->GetActorLocation();
	FVector vehicleLocation = GetOwner()->GetActorLocation();
	float distance = FVector::Distance(checkpointLocation, vehicleLocation);

	FVector direction = checkpointLocation - vehicleLocation;
	direction.Normalize();
	float angle = FMath::Acos(FVector::DotProduct(GetOwner()->GetActorForwardVector(), direction));

	return FMath::Abs(distance * FMath::Cos(angle));
}

float URaceComponent::GetTimeValue()
{
	return generalLapTime;
}

bool URaceComponent::InFrontOfOpponent(URaceComponent* opponent)
{
	return GetCurrentLap() > opponent->GetCurrentLap() ||
		GetCurrentLap() == opponent->GetCurrentLap() &&
		(GetCheckpointPosition() > opponent->GetCheckpointPosition() ||
			GetCheckpointPosition() == opponent->GetCheckpointPosition() && GetDistance() < opponent->GetDistance());
}

FString URaceComponent::GenerateTimeString(float time)
{
	if (time >= 0)
	{
		int32 minutes = time / 60;
		int32 seconds = time;
		seconds %= 60;
		int32 milliseconds = (time - ((minutes * 60) + seconds)) * 1000;

		FString minutesString = minutes < 10
			                        ? FString::Printf(TEXT("0%i"), minutes)
			                        : FString::Printf(TEXT("%i"), minutes);
		FString secondsString = seconds < 10
			                        ? FString::Printf(TEXT("0%i"), seconds)
			                        : FString::Printf(TEXT("%i"), seconds);
		FString millisecondsString = milliseconds < 10
			                             ? FString::Printf(TEXT("00%i"), milliseconds)
			                             : milliseconds < 100
			                             ? FString::Printf(TEXT("0%i"), milliseconds)
			                             : FString::Printf(TEXT("%i"), milliseconds);
		return FString::Printf(TEXT("%s:%s:%s"), *minutesString, *secondsString, *millisecondsString);
	}
	return TEXT("NO TIME");
}

float URaceComponent::GetCurrentLapTime() const
{
	return currentLapTime;
}

float URaceComponent::GetBestLapTime() const
{
	return bestLapTime;
}

bool URaceComponent::CanRace() const
{
	if (!gameInstance->GetLevel())
		return false;
	return gameInstance->GetLevel()->GetCanRace();
}

void URaceComponent::SetCanRace(bool value) const
{
	if (!gameInstance->GetLevel())
		return;
	gameInstance->GetLevel()->SetCanRace(value);
}

bool URaceComponent::CanStartRace() const
{
	if (!gameInstance->GetLevel())
		return false;
	return gameInstance->GetLevel()->GetCanStartRace();
}


void URaceComponent::Ragnarok(int racePosition)
{
	if (!finished)
	{
		checkpoint = gameInstance->GetLevel()->GetCheckpoint(0);
		currentLap = maxLaps - 1;
		currentLapTime = 0;
		vehicle->SetSpeed(0);
		racePosition = (gameInstance->GetLevel()->GetNumberOfVehicles() - 1) - racePosition;
		vehicle->SetActorLocation(gameInstance->GetLevel()->GetRaceStart(racePosition)->GetActorLocation());
		vehicle->SetActorRotation(gameInstance->GetLevel()->GetRaceStart(racePosition)->GetActorRotation());
		vehicle->GetNetworkComponent()->SendMessage();
	}
}

int32 URaceComponent::GetCheckpointPosition() const
{
	return checkpoint->GetPosition();
}

void URaceComponent::SetCheckpoint(ACheckPoint* newCheckpoint)
{
	checkpoint = newCheckpoint;
}

FString URaceComponent::GetUsername()
{
	return vehicle->GetNetworkComponent()->GetUsername();
}

bool URaceComponent::HasFinished()
{
	return finished;
}

void URaceComponent::Destroy()
{
	GetOwner()->Destroy();
}

void URaceComponent::SetTime(float networkTime)
{
	generalLapTime = networkTime;
}
