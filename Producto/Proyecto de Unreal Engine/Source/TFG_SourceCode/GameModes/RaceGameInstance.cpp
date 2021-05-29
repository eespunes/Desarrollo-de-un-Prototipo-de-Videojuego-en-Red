// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#include "RaceGameInstance.h"
#include "JsonObjectConverter.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/Components/NetworkComponent.h"


void URaceGameInstance::Init()
{
	Super::Init();

	networkController = NewObject<UNetworkController>();

	if (networkController)
	{
		networkController->AddToRoot();
		networkController->SetGameInstance(this);
	}

	objectsProbabilities = {
		FObjectProbabilityStruct{
			{0.25, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.15, 0.1, 0.05, 0.05, 0.05}
		},
		FObjectProbabilityStruct{
			{0.6, 0.2, 0.15, 0.1, 0.05, 0.05, 0, 0, 0, 0, 0, 0}
		},
		FObjectProbabilityStruct{
			{0.05, 0.15, 0.2, 0.25, 0.25, 0.25, 0.2, 0.15, 0.1, 0.05, 0.05, 0.05}
		},
		FObjectProbabilityStruct{
			{0.05, 0.15, 0.2, 0.15, 0.15, 0.1, 0.1, 0.1, 0.1, 0.05, 0.05, 0.05}
		},
		FObjectProbabilityStruct{
			{0, 0, 0, 0, 0, 0.03, 0.05, 0.1, 0.1, 0.15, 0.1, 0.1}
		},
		FObjectProbabilityStruct{
			{0, 0, 0, 0, 0, 0.02, 0.1, 0.1, 0.1, 0.15, 0.15, 0.15}
		},
		FObjectProbabilityStruct{
			{0, 0, 0, 0, 0, 0, 0.1, 0.1, 0.15, 0.2, 0.3, 0.39}
		},
		FObjectProbabilityStruct{
			{0, 0, 0, 0.05, 0.05, 0.1, 0.1, 0.15, 0.2, 0.25, 0.2, 0.15}
		},
		FObjectProbabilityStruct{
			{0.05, 0.1, 0.1, 0.15, 0.25, 0.25, 0.2, 0.15, 0.15, 0.1, 0.1, 0.05}
		},
		FObjectProbabilityStruct{
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.01}
		}
	};
}

void URaceGameInstance::Shutdown()
{
	Super::Shutdown();
	
	DisconnectSocket();
}

//DIFFICULTY
float URaceGameInstance::GetDifficulty() const
{
	if (currentDifficulty == 0)
		return lowerDifficulty;
	if (currentDifficulty == 1)
		return mediumDifficulty;

	return higherDifficulty;
}

void URaceGameInstance::SetCurrentDifficulty(int CurrentDifficulty)
{
	currentDifficulty = CurrentDifficulty;
}

//API REQUESTS
void URaceGameInstance::SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json)
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance"));
		return;
	}
	networkController->SendPost(request, uri, json);
}

bool URaceGameInstance::IsSuccessful(int32 responseCode)
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance"));
		return false;
	}
	return networkController->IsSuccessful(responseCode);
}

void URaceGameInstance::SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri)
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance"));
		return;
	}
	networkController->SendGet(request, uri);
}

//SOCKET.IO REQUESTS
void URaceGameInstance::ConnectSocket()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Connection"));
		return;
	}
	networkController->ConnectSocket();
}

void URaceGameInstance::ReceiveFindRaceEvents()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Receive FindRace"));
		return;
	}
	networkController->ReceiveFindRaceEvents();
}

void URaceGameInstance::SendEvents(FString room, FString json)
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Send"));
		return;
	}
	networkController->SendEvents(room, json);
}

void URaceGameInstance::SendRaceEvents(FRacingStruct& racingStruct)
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance SendRace"));
		return;
	}
	FString json;
	FJsonObjectConverter::UStructToJsonObjectString(FRacingStruct::StaticStruct(), &racingStruct, json, 0, 0);

	networkController->SendEvents("Racing", json);
}

bool URaceGameInstance::SocketIsConnected()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance IsConnected"));
		return false;
	}

	return networkController->SocketIsConnected();
}

FString URaceGameInstance::GetRaceID()
{
	return raceID;
}

void URaceGameInstance::SetRaceID(FString race)
{
	raceID = race;
}

bool URaceGameInstance::GetRaceFound()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance RaceFound"));
		return false;
	}
	bool bRaceFound = networkController->GetRaceFound();
	if (bRaceFound)
		SetRaceID(networkController->GetRaceID());
	return bRaceFound;
}

bool URaceGameInstance::GetRaceFoundSuccessfully()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Timer"));
		return false;
	}
	return networkController->GetRaceFoundSuccessfully();
}

FString URaceGameInstance::Timer()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Timer"));
		return "";
	}
	return networkController->Timer();
}

TArray<FString> URaceGameInstance::GetPlayersInRace()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance GetPlayersInRace"));
		TArray<FString> empty;
		return empty;
	}
	return networkController->GetPlayersInRace();
}

bool URaceGameInstance::GetPlayersUpdated()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance GetPlayersUpdated"));
		return false;
	}
	return networkController->GetPlayersUpdated();
}

void URaceGameInstance::DisconnectSocket()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Disconnect"));
		return;
	}
	networkController->DisconnectRace(player->pla_username);
	networkController->DisconnectSocket();
}

void URaceGameInstance::SetPlayer(FPlayerStruct* PlayerStruct)
{
	player = PlayerStruct;
}

void URaceGameInstance::SetMultiplayer(bool multiplayer)
{
	isMultiplayer = multiplayer;
}

bool URaceGameInstance::IsMultiplayer()
{
	return isMultiplayer;
}

FPlayerStruct URaceGameInstance::GetPlayer()
{
	return player;
}

ARacingLevel* URaceGameInstance::GetLevel()
{
	return level;
}

void URaceGameInstance::SetLevel(ARacingLevel* other)
{
	this->level = other;
}

void URaceGameInstance::AddNetworkPlayers(TArray<AVehiclePawn*> Array)
{
	this->networkPlayers = Array;
}

AVehiclePawn* URaceGameInstance::FindPlayer(const FString& Username)
{
	for (auto vehiclePawn : networkPlayers)
	{
		if (vehiclePawn->GetNetworkComponent()->username == Username)
			return vehiclePawn;
	}
	return nullptr;
}

TSubclassOf<UObject> URaceGameInstance::GetObject(int32 idx)
{
	return objects[idx % objects.Num()]->GeneratedClass;
}

TSubclassOf<UObject> URaceGameInstance::GetRandomObject(int32 position)
{
	int realPosition = position - 1;
	float probability = FMath::RandRange(0.f, 1.f);

	float objectProbabilityValue = 0;

	for (int i = 0; i < objects.Num(); ++i)
	{
		objectProbabilityValue += objectsProbabilities[i].probabilities[realPosition];

		if (probability <= objectProbabilityValue) return objects[i]->GeneratedClass;
	}
	return objects[0]->GeneratedClass;
}

int32 URaceGameInstance::GetObjectsSize()
{
	return objects.Num();
}

void URaceGameInstance::DisconnectRace()
{
	if (!networkController)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no UNetworkController instance Disconnect"));
		return;
	}
	networkController->DisconnectRace(player->pla_username);
}
