// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "NetworkController.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "SocketIOClient.h"
#include "Interfaces/IHttpRequest.h"
#include "TFG_SourceCode/Widgets/MainMenuWidget.h"
#include "SocketIOClientComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"

UNetworkController::UNetworkController()
{
}

void UNetworkController::SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json)
{
	request->SetURL(URL + uri);
	request->SetVerb("POST");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->SetContentAsString(json);
	request->ProcessRequest();
}

bool UNetworkController::IsSuccessful(int32 responseCode)
{
	return responseCode / 100 == 2;
}

void UNetworkController::SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri)
{
	request->SetURL(URL + uri);
	request->SetVerb("GET");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UNetworkController::ConnectSocket()
{
	socket = ISocketIOClientModule::Get().NewValidNativePointer();
	if (socket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket created"));
		socket->Connect(TEXT("http://localhost:3000"), nullptr, nullptr);
		UE_LOG(LogTemp, Warning, TEXT("SocketID: %s"), * socket->SessionId);
	}
}

void UNetworkController::ReceiveFindRaceEvents()
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	socket->OnEvent("FindRace", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		raceFoundSuccessfully = false;
		TSharedPtr<FJsonObject> json = Message->AsObject();
		// UE_LOG(LogTemp, Log, TEXT("Received FindRace: %s"), *USIOJConvert::ToJsonString(Message));

		FRaceStruct raceStruct = new FRaceStruct();
		raceStruct.race = json->GetStringField("race");
		raceStruct.status = json->GetIntegerField("status");

		// UE_LOG(LogTemp, Log, TEXT("%i: %s"), raceStruct.status, *raceStruct.race);

		raceFound = true;
		if (IsSuccessful(raceStruct.status))
		{
			ReceiveRaceEvents(raceStruct.race);
			raceId = raceStruct.race;
			raceFoundSuccessfully = true;
		}
	});
}

void UNetworkController::ReceiveRaceEvents(FString raceID)
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("RaceID: %s"), *raceID);

	socket->OnEvent(raceID + "-players", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		//TODO CHANGE THIS
		UE_LOG(LogTemp, Log, TEXT("Received Race: %s"), *USIOJConvert::ToJsonString(Message));
		FString array = *USIOJConvert::ToJsonString(Message);
		array = array.Replace(TEXT("["), TEXT(""));
		array = array.Replace(TEXT("]"), TEXT(""));
		array = array.Replace(TEXT("\""), TEXT(""));
		UE_LOG(LogTemp, Log, TEXT("Received RaceID: %s"), *array);
		array.ParseIntoArray(this->playersInRace,TEXT(","), true);
		playersArrayUpdated = true;
	});
	socket->OnEvent(raceID + "-timer", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		timer = *USIOJConvert::ToJsonString(Message);
	});
	socket->OnEvent(raceID + "-start", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Received Race: %s"), *USIOJConvert::ToJsonString(Message));
		if (gameInstance)
		{
			ARacingLevel* level = gameInstance->GetLevel();
			if (level)
			{
				level->SetCanStartRace(true);
			}
		}
	});
	socket->OnEvent(raceID, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		UE_LOG(LogTemp, Log, TEXT("Racing: %s"), *USIOJConvert::ToJsonString(Message));
	});
}

void UNetworkController::SendEvents(FString room, FString json)
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}
	raceFound = false;
	socket->Emit(room, json);
}

bool UNetworkController::SocketIsConnected()
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return false;
	}

	return socket->bIsConnected;
}

bool UNetworkController::GetRaceFound()
{
	return raceFound;
}

bool UNetworkController::GetRaceFoundSuccessfully()
{
	return raceFoundSuccessfully;
}

FString UNetworkController::Timer()
{
	return timer;
}

TArray<FString> UNetworkController::GetPlayersInRace()
{
	playersArrayUpdated = false;
	return playersInRace;
}

bool UNetworkController::GetPlayersUpdated()
{
	return playersArrayUpdated;
}

void UNetworkController::DisconnectSocket()
{
	if (socket.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("DISCONNECT"));
		socket->Disconnect();
		ISocketIOClientModule::Get().ReleaseNativePointer(socket);
		socket = nullptr;
	}
}

FString UNetworkController::GetRaceID()
{
	return raceId;
}

void UNetworkController::SetGameInstance(URaceGameInstance* instance)
{
	gameInstance = instance;
}
