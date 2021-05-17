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
	UE_LOG(LogTemp, Warning, TEXT("Init: %s"), *GetName());

	networkController = NewObject<UNetworkController>();

	if (networkController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Network Controller created: %s"), *networkController->GetName());
		networkController->AddToRoot();
		networkController->SetGameInstance(this);
	}
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
	UE_LOG(LogTemp, Error, TEXT("HOla: %s"), *race);
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
