// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"


#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void ARaceGameMode::NormalSpawn(APlayerStart*& FoundPlayerStart)
{
	FoundPlayerStart = nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		UnOccupiedStartPoints.Add(*It);
	}

	APlayerStart* aux;
	for (int i = 1; i < UnOccupiedStartPoints.Num(); i++)
	{
		for (int j = UnOccupiedStartPoints.Num() - 1; j >= i; j--)
		{
			if (UnOccupiedStartPoints[j]->GetName() < UnOccupiedStartPoints[j - 1]->
				GetName())
			{
				aux = UnOccupiedStartPoints[j];
				UnOccupiedStartPoints[j] = UnOccupiedStartPoints[j - 1];
				UnOccupiedStartPoints[j - 1] = aux;
			}
		}
	}

	int32 playerStartPosition = (playerPosition - 1) % UnOccupiedStartPoints.Num();
	FoundPlayerStart = UnOccupiedStartPoints[playerStartPosition];

	// for (int i = 0; i < UnOccupiedStartPoints.Num(); ++i)
	// {
	// 	if (i != playerStartPosition)
	// 	{
	// 		GetWorld()->SpawnActor<AVehiclePawn>(
	// 			vehicleToSpawn,
	// 			UnOccupiedStartPoints[i]->GetActorLocation(),
	// 			UnOccupiedStartPoints[i]->GetActorRotation()
	// 			)->SpawnDefaultController();
	// 	}
	// }
}

AActor* ARaceGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* FoundPlayerStart;
	NormalSpawn(FoundPlayerStart);
	return FoundPlayerStart;
}
