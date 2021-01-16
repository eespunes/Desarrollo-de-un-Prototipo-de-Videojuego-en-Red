// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "RaceComponent.h"

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

	level = Cast<ABaseLevelActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABaseLevelActor::StaticClass()));
	if (!level)
	UE_LOG(LogTemp, Error, TEXT("There's no ABaseLevelActor instance in %s"), *GetOwner()->GetActorLabel());
}


// Called every frame
void URaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentLapTime += DeltaTime;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green,
                                 FString::Printf(
                                     TEXT("Current Lap Time: %f"), currentLapTime));
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green,
		                                 FString::Printf(
			                                 TEXT("Current Lap: %i"), currentLap));
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green,
                                 FString::Printf(
                                     TEXT("Current Checkpoint: %i"), currentCheckPoint));
	}
}

void URaceComponent::SetCheckpoint(int checkpoint)
{
	if (checkpoint == expectedCheckpoint)
	{
		currentCheckPoint = checkpoint;
		if (currentCheckPoint == level->GetNumberOfCheckpoints() - 1)
		{
			expectedCheckpoint = 0;
			currentLap++;
			currentLapTime = 0;
		}
		else
			expectedCheckpoint = currentCheckPoint + 1;
	}
}
