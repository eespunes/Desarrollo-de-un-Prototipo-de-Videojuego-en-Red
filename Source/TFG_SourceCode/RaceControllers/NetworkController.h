// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "SocketIOClient.h"
#include "SocketIONative.h"
#include "UObject/NoExportTypes.h"
#include "NetworkController.generated.h"

class URaceGameInstance;
class IHttpRequest;
class FHttpModule;
class USocketIOClientComponent;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UNetworkController : public UObject
{
	GENERATED_BODY()
public:
	UNetworkController();
	void SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json);
	bool IsSuccessful(int32 responseCode);
	void SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri);

	void ConnectSocket();
	void ReceiveFindRaceEvents();
	void ReceiveRaceEvents(FString raceID);
	void SendEvents(FString room, FString json);
	bool SocketIsConnected();
	bool GetRaceFound();
	bool GetRaceFoundSuccessfully();
	FString Timer();
	TArray<FString> GetPlayersInRace();
	bool GetPlayersUpdated();
	void DisconnectRace(FString username);
	void DisconnectSocket();
	FString GetRaceID();
	void SetGameInstance(URaceGameInstance* instance);
private:
	const FString URL = "http://tfg-videojocs.herokuapp.com/";
	// const FString URL = "http://localhost:3000/";
	FHttpModule* http;
	TSharedPtr<FSocketIONative> socket;
	bool raceFound;
	bool raceFoundSuccessfully;
	bool playersArrayUpdated;
	TArray<FString> playersInRace;
	FString timer;
	FString raceId;
	URaceGameInstance* gameInstance;
};

USTRUCT()
struct TFG_SOURCECODE_API FRaceStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString race;
	UPROPERTY()
	int32 status;

	FRaceStruct()
	{
	};

	FRaceStruct(FRaceStruct* other)
	{
		this->race = other->race;
		this->status = other->status;
	};
};
