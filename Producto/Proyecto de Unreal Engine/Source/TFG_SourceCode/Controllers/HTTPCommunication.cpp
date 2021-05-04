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
	UE_LOG(LogTemp, Error, TEXT("WTF? %s"), *(URL + uri));
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
	UE_LOG(LogTemp, Error, TEXT("URL: %s%s"), *URL, *uri)
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

void UHTTPCommunication::ReceiveEvents()
{
	if (!socket)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no USocketIOClientComponent instance"));
		return;
	}

	socket->OnEvent("FindRace", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{	
		FString json=*USIOJConvert::ToJsonString(Message);
		UE_LOG(LogTemp, Log, TEXT("Received: %s"), *json);
		TArray<FRaceStruct> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(json, &data, 0, 0);
		if (data.Num() > 0)
		{
		FRaceStruct raceStruct = new FRaceStruct(&data[0]);
		UE_LOG(LogTemp, Warning, TEXT("%i,%s"), raceStruct.status, *raceStruct.race)
		if (IsSuccessful(raceStruct.status))
			raceID = raceStruct.race;
		}
		raceFound = true;
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
