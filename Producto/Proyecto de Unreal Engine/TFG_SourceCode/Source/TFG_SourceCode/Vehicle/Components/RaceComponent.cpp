// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "RaceComponent.h"

#include <string>


#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/BaseLevelActor.h"

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

	ABaseLevelActor* level = Cast<ABaseLevelActor>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ABaseLevelActor::StaticClass()));
	if (level)
	{
		maxCheckpoints = level->GetNumberOfCheckpoints();
		maxLaps = level->GetNumberOfLaps();
	}
}


// Called every frame
void URaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentLapTime += DeltaTime;
}

void URaceComponent::SetCheckpoint(int checkpoint)
{
	if (checkpoint == expectedCheckpoint)
	{
		currentCheckPoint = checkpoint;
		if (currentCheckPoint == maxCheckpoints - 1)
		{
			expectedCheckpoint = 0;
			currentLap++;
			if (bestLapTime == -1 || currentLapTime <= bestLapTime)
				bestLapTime = currentLapTime;
			currentLapTime = 0;
		}
		else
			expectedCheckpoint = currentCheckPoint + 1;
	}
}

int URaceComponent::GetPosition() const
{
	return position;
}

int URaceComponent::GetExpectedCheckpoint() const
{
	return expectedCheckpoint;
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

FString URaceComponent::GenerateTimeString(float time) const
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
	return TEXT("00:00:00");
}

float URaceComponent::GetCurrentLapTime() const
{
	return currentLapTime;
}

float URaceComponent::GetBestLapTime() const
{
	return bestLapTime;
}

void URaceComponent::CalculateDistance(ACheckPoint* checkpoint)
{
	distance = (currentLap + 1) * (currentCheckPoint >= 0 ? currentCheckPoint + 1 : 1) * FVector::Distance(
		GetOwner()->GetActorLocation(), checkpoint->GetTargetLocation());
}
