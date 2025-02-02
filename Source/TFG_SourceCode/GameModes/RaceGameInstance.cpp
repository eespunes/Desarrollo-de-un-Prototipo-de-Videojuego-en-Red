// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#include "RaceGameInstance.h"
#include "JsonObjectConverter.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/RaceControllers/NetworkController.h"


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
	if (IsMultiplayer())
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
		return;
	}
	FString json;
	FJsonObjectConverter::UStructToJsonObjectString(FRacingStruct::StaticStruct(), &racingStruct, json, 0, 0);

	networkController->SendEvents("Racing", json);
}

void URaceGameInstance::SendObjectEvents(const FString& json)
{
	if (!networkController)
	{
		return;
	}
	networkController->SendEvents("Object", json);
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
	if (player)
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
	for (int i = 0; i < level->GetNumberOfVehicles(); ++i)
	{
		if (level->GetVehicleByPosition(i))
		{
			AVehiclePawn* vehicle = Cast<AVehiclePawn>(level->GetVehicleByPosition(i)->GetOwner());
			if (vehicle && vehicle->GetNetworkComponent()->username == Username)
				return vehicle;
		}
	}
	return nullptr;
}

TSubclassOf<UObject> URaceGameInstance::GetObject(int32 idx)
{
	return objects[idx % objects.Num()]->GeneratedClass;
}

int32 URaceGameInstance::GetRandomObject(int32 position)
{
	if (isMultiplayer)
	{
		int realPosition = position - 1;
		float probability = FMath::RandRange(0.f, 1.f);

		float objectProbabilityValue = 0;

		for (int i = 0; i < objectsProbabilities.Num(); ++i)
		{
			objectProbabilityValue += objectsProbabilities[i].probabilities[realPosition];

			if (probability <= objectProbabilityValue) return i;
		}
	}

	return 2;
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

void URaceGameInstance::SetPauseMenuActivated(bool activated)
{
	pauseMenuActivated = activated;
}

bool URaceGameInstance::GetPauseMenuActivated() const
{
	return pauseMenuActivated;
}

void URaceGameInstance::SetClassification(TArray<FString> stringArray)
{
	TArray<URaceComponent*> raceComponentArray = level->GetVehicles();
	TArray<URaceComponent*> classification;

	if (raceComponentArray.Num() != stringArray.Num())
	{
		int32 index = 0;
		for (auto raceComponent : raceComponentArray)
		{
			bool found = false;
			for (auto string : stringArray)
			{
				if (string.Equals(raceComponent->GetUsername()))
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				raceComponentArray.RemoveAt(index);
				raceComponent->Destroy();
			}
			index++;
		}
	}

	int position = 0;
	for (auto string : stringArray)
	{
		for (int i = 0; i < raceComponentArray.Num(); ++i)
		{
			if (raceComponentArray[i]->GetUsername().Equals(string))
			{
				raceComponentArray[i]->SetPosition(i + 1);
				classification.Add(raceComponentArray[i]);
				break;
			}
		}
	}
	level->SetClassification(classification);
}

TArray<URaceComponent*> URaceGameInstance::GetClassification()
{
	return level->GetVehicles();
}

int32 URaceGameInstance::GetNextObjectID()
{
	return objectID++;
}

void URaceGameInstance::AddObjectInGame(AObjectBase* ObjectBase)
{
	objectsInGame.Add(ObjectBase);
}

void URaceGameInstance::RemoveObjectInGame(AObjectBase* ObjectBase)
{
	objectsInGame.Remove(ObjectBase);
}

AObjectBase* URaceGameInstance::GetObjectInGame(FString username, int32 id)
{
	for (auto object : objectsInGame)
	{
		if (object->GetUsername().Equals(username) && object->GetID() == id)
			return object;
	}
	return nullptr;
}

void URaceGameInstance::Teleport(ACheckPoint* checkPoint, AVehiclePawn* vehicle, const FVector& offset)
{
	FVector position = checkPoint->GetActorLocation();
	FRotator rotation = checkPoint->GetActorRotation();
	FVector rotationEuler = rotation.Euler();
	rotation = FRotator::MakeFromEuler(
		FVector(rotationEuler.X, rotationEuler.Y, rotationEuler.Z-180)/* - checkPoint->GetActorUpVector() * 180*/);

	vehicle->GetRaceComponent()->SetCheckpoint(checkPoint);

	position += offset.Size() * checkPoint->GetActorUpVector();
	vehicle->SetActorLocation(position, false, nullptr, ETeleportType::TeleportPhysics);
	vehicle->SetActorRotation(rotation, ETeleportType::TeleportPhysics);
	
	vehicle->SetSpeed(0);
	vehicle->ResetForces(-checkPoint->GetActorForwardVector());
}
