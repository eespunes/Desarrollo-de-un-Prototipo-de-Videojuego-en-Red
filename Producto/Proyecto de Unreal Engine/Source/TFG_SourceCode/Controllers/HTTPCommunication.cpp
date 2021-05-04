// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "HTTPCommunication.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "SocketIOClient.h"
#include "Interfaces/IHttpRequest.h"
#include "TFG_SourceCode/Widgets/MainMenuWidget.h"
#include "SocketIOClientComponent.h"

UHTTPCommunication::UHTTPCommunication()
{
	socket = ISocketIOClientModule::Get().NewValidNativePointer();
	if (socket)
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket created"));
	}
}

void UHTTPCommunication::SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json)
{
	request->SetURL(URL + uri);
	request->SetVerb("POST");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->SetContentAsString(json);
	request->ProcessRequest();
}

bool UHTTPCommunication::IsSuccessful(int32 responseCode)
{
	return responseCode / 100 == 2;
}

void UHTTPCommunication::SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri)
{
	request->SetURL(URL + uri);
	request->SetVerb("GET");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UHTTPCommunication::ConnectSocket()
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}
	socket->Connect(TEXT("http://localhost:3000"), nullptr, nullptr);
}

void UHTTPCommunication::ReceiveFindRaceEvents()
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	socket->OnEvent("FindRace", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		TSharedPtr<FJsonObject> json = Message->AsObject();
		UE_LOG(LogTemp, Log, TEXT("Received FindRace: %s"), *USIOJConvert::ToJsonString(Message));

		FRaceStruct raceStruct = new FRaceStruct();
		raceStruct.race = json->GetStringField("race");
		raceStruct.status = json->GetIntegerField("status");

		UE_LOG(LogTemp, Log, TEXT("%i: %s"), raceStruct.status, *raceStruct.race);

		if (IsSuccessful(raceStruct.status))
		{
			raceID = raceStruct.race;
			ReceiveRaceEvents();
		}
		raceFound = true;
	});
}

void UHTTPCommunication::ReceiveRaceEvents()
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("RaceID: %s"), *raceID);

	socket->OnEvent(raceID, [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		// UE_LOG(LogTemp, Log, TEXT("Received RaceID: %s"), *USIOJConvert::ToJsonString(Message));
		FString array = *USIOJConvert::ToJsonString(Message);
		array = array.Replace(TEXT("["), TEXT(""));
		array = array.Replace(TEXT("]"), TEXT(""));
		array = array.Replace(TEXT("\""), TEXT(""));
		UE_LOG(LogTemp, Log, TEXT("Received RaceID: %s"), *array);
		array.ParseIntoArray(this->playersInRace,TEXT(","), true);
		playersArrayUpdated = true;
	});
}

void UHTTPCommunication::SendEvents(FString room, FString json)
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	socket->Emit(room, json);
}

bool UHTTPCommunication::SocketIsConnected()
{
	return socket->bIsConnected;
}

FString UHTTPCommunication::GetRaceID()
{
	return raceID;
}

bool UHTTPCommunication::RaceFound()
{
	return raceFound;
}

TArray<FString> UHTTPCommunication::GetPlayersInRace()
{
	playersArrayUpdated = false;
	return playersInRace;
}

bool UHTTPCommunication::GetPlayersUpdated()
{
	return playersArrayUpdated;
}

void UHTTPCommunication::DisconnectSocket()
{
	if (socket.IsValid())
	{
		socket->Disconnect();
		ISocketIOClientModule::Get().ReleaseNativePointer(socket);
		socket = nullptr;
	}
}
