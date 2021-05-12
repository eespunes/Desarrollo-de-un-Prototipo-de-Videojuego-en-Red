// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameMode.h"


#include "EngineUtils.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/NetworkComponent.h"

AActor* ARaceGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* FoundPlayerStart = nullptr;
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInstance)
	{
		if (gameInstance->IsMultiplayer())
		{
			MultiplayerSpawn(FoundPlayerStart);
		}
		else
		{
			SingleplayerSpawn(FoundPlayerStart);
		}
	}
	return FoundPlayerStart;
}

void ARaceGameMode::SingleplayerSpawn(APlayerStart*& FoundPlayerStart)
{
	TArray<APlayerStart*> startPoints;

	FindStartPoints(startPoints);

	OrderStartPoints(startPoints);

	int32 playerStartPosition = (playerPosition - 1) % startPoints.Num();
	FoundPlayerStart = startPoints[playerStartPosition];

	// for (int i = 0; i < startPoints.Num(); ++i)
	// {
	// 	if (i != playerStartPosition)
	// 	{
	// 		GetWorld()->SpawnActor<AVehiclePawn>(
	// 			vehicleToSpawn,
	// 			startPoints[i]->GetActorLocation(),
	// 			startPoints[i]->GetActorRotation()
	// 			)->SpawnDefaultController();
	// 	}
	// }
}

void ARaceGameMode::MultiplayerSpawn(APlayerStart*& FoundPlayerStart)
{
	TArray<APlayerStart*> startPoints;

	FindStartPoints(startPoints);

	OrderStartPoints(startPoints);

	TArray<FString> playersInRace = gameInstance->GetPlayersInRace();

	for (int i = 0; i < playersInRace.Num(); ++i)
	{
		if (playersInRace[i].Equals(gameInstance->GetPlayer().pla_username))
		{
			FoundPlayerStart = startPoints[i];
		}
		else
		{
			AVehiclePawn* vehiclePawn = GetWorld()->SpawnActor<AVehiclePawn>(
				vehicleToSpawn,
				startPoints[i]->GetActorLocation(),
				startPoints[i]->GetActorRotation()
			);
			vehiclePawn->AIControllerClass = multiplayerController;
			vehiclePawn->GetNetworkComponent()->SetUsername(playersInRace[i]);
			vehiclePawn->SpawnDefaultController();
		}
	}
}

void ARaceGameMode::FindStartPoints(TArray<APlayerStart*>& startPoints)
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		startPoints.Add(*It);
	}
}

void ARaceGameMode::OrderStartPoints(TArray<APlayerStart*>& UnOccupiedStartPoints)
{
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
}
