// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "NetworkController.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "SocketIOClient.h"
#include "Interfaces/IHttpRequest.h"
#include "TFG_SourceCode/Widgets/MainMenuWidget.h"
#include "SocketIOClientComponent.h"
#include "Engine/SpringInterpolator.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/Components/NetworkComponent.h"
#include "TFG_SourceCode/Vehicle/Controllers/VehicleController.h"
#include "TFG_SourceCode/Vehicle/Controllers/VehicleNetworkController.h"

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
		socket->Connect(URL, nullptr, nullptr);
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

		FRaceStruct raceStruct = new FRaceStruct();
		raceStruct.race = json->GetStringField("race");
		raceStruct.status = json->GetIntegerField("status");

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
		FString array = *USIOJConvert::ToJsonString(Message);
		array = array.Replace(TEXT("["), TEXT(""));
		array = array.Replace(TEXT("]"), TEXT(""));
		array = array.Replace(TEXT("\""), TEXT(""));
		array.ParseIntoArray(this->playersInRace,TEXT(","), true);
		playersArrayUpdated = true;
	});
	socket->OnEvent(raceID + "-timer", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
		timer = *USIOJConvert::ToJsonString(Message);
	});
	socket->OnEvent(raceID + "-start", [this](const FString& Event, const TSharedPtr<FJsonValue>& Message)
	{
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
		                TSharedPtr<FJsonObject> jsonObject = Message->AsObject();
		                FString username = jsonObject->GetStringField("username");

		                AVehiclePawn* player = gameInstance->FindPlayer(username);
		                if (player)
		                {
			                if (Cast<AVehicleNetworkController>(player->GetController()))
			                {
				                FVector networkPosition = FVector(jsonObject->GetNumberField("positionX"),
				                                                  jsonObject->GetNumberField("positionY"),
				                                                  jsonObject->GetNumberField("positionZ"));

				                player->SetActorLocation(networkPosition);

				                FRotator networkRotation = FRotator(jsonObject->GetNumberField("rotationPitch"),
				                                                    jsonObject->GetNumberField("rotationYaw"),
				                                                    jsonObject->GetNumberField("rotationRoll"));
				                player->SetActorRotation(networkRotation);

				                player->SetSpeed(jsonObject->GetNumberField("speed"));

				                player->SetIsAccelerating(jsonObject->GetBoolField("isAccelerating"));
				                player->SetIsBraking(jsonObject->GetBoolField("isBraking"));
				                player->SetSteerValue(jsonObject->GetNumberField("turnValue"));
				                player->SetIsDrifting(jsonObject->GetBoolField("isDrifting"));
				                player->SetDriftValue(jsonObject->GetNumberField("driftValue"));

				                player->SetAccelerationTimer(jsonObject->GetNumberField("accelerationTimer"));
				                player->SetBrakeTimer(jsonObject->GetNumberField("brakingTimer"));
				                player->SetDecelerationTimer(jsonObject->GetNumberField("decelerationTimer"));
				                player->SetDriftIncreaseTimer(jsonObject->GetNumberField("driftIncreaseTimer"));
				                player->SetDriftDecreaseIncreaseTimer(jsonObject->GetNumberField("driftDecreaseTimer"));

				                int32 object = jsonObject->GetIntegerField("object");
				                if (!player->GetCurrentObject() && object != -1)
				                {
					                player->SetCurrentObject(gameInstance->GetObject(object));
				                }

				                if (jsonObject->GetBoolField("useObject"))
				                {
					                player->UseObject();
				                }
			                }
			                else
			                {
				                player->GetNetworkComponent()->SetPing(
					                (player->GetRaceComponent()->GetTimeValue() - jsonObject->GetNumberField("time")) *
					                1000);
			                }
		                }
	                }
	);
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

void UNetworkController::DisconnectRace(FString username)
{
	FString json;
	FFindRaceStruct findRaceStruct;
	findRaceStruct.username = username;
	findRaceStruct.levelID = raceId;

	FJsonObjectConverter::UStructToJsonObjectString(FFindRaceStruct::StaticStruct(), &findRaceStruct, json, 0, 0);
	SendEvents(TEXT("Disconnect"), json);
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
