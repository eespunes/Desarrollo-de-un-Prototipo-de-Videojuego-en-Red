// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLevelActor.h"

#include "Kismet/GameplayStatics.h"

void ABaseLevelActor::BeginPlay()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), foundActors);

	int c = 0;
	for (AActor* actor : foundActors)
	{
		ACheckPoint* checkpoint = Cast<ACheckPoint>(actor);
		if (checkpoint)
		{
			++c;
		}
	}
	numberOfCheckpoints = c;
}

int ABaseLevelActor::GetNumberOfLaps() const
{
	return numberOfLaps;
}

int ABaseLevelActor::GetNumberOfCheckpoints() const
{
	return numberOfCheckpoints;
}