// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/Widgets/MainMenuWidget.h"

#include "RaceGameInstance.generated.h"

class UNetworkController;
class AObjectBase;
struct FObjectProbabilityStruct;
struct FRacingStruct;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API URaceGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Difficulty")
	float lowerDifficulty = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Difficulty")
	float mediumDifficulty = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Difficulty")
	float higherDifficulty = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objects")
	TArray<UBlueprint*> objects;
	TArray<FObjectProbabilityStruct> objectsProbabilities;

	TArray<AObjectBase*> objectsInGame;

	int currentDifficulty = -1;

	FString raceID;

	UNetworkController* networkController;

	bool isMultiplayer;
	FPlayerStruct* player;
	ARacingLevel* level;
	TArray<AVehiclePawn*> networkPlayers;
	bool pauseMenuActivated;
	int32 objectID;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	float GetDifficulty() const;
	UFUNCTION(BlueprintCallable)
	void SetCurrentDifficulty(int CurrentDifficulty);

	void SendPost(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, FString uri, FString json);
	bool IsSuccessful(int32 responseCode);
	void SendGet(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request, const FString& uri);

	void ConnectSocket();
	void ReceiveFindRaceEvents();
	void SendEvents(FString room, FString json);
	void SendRaceEvents(FRacingStruct& json);
	void SendObjectEvents(const FString& json);
	bool SocketIsConnected();
	FString GetRaceID();
	void SetRaceID(FString race);
	bool GetRaceFound();
	bool GetRaceFoundSuccessfully();
	FString Timer();
	TArray<FString> GetPlayersInRace();
	bool GetPlayersUpdated();
	UFUNCTION(BlueprintCallable)
	void DisconnectSocket();
	void SetPlayer(FPlayerStruct* PlayerStruct);
	UFUNCTION(BlueprintCallable)
	void SetMultiplayer(bool multiplayer);
	UFUNCTION(BlueprintPure)
	bool IsMultiplayer();
	FPlayerStruct GetPlayer();
	UFUNCTION(BlueprintPure)
	ARacingLevel* GetLevel();
	void SetLevel(ARacingLevel* other);
	void AddNetworkPlayers(TArray<AVehiclePawn*> Array);
	AVehiclePawn* FindPlayer(const FString& Username);

	TSubclassOf<UObject> GetObject(int32 idx);
	int32 GetRandomObject(int32 position);
	int32 GetObjectsSize();
	UFUNCTION(BlueprintCallable)
	void DisconnectRace();
	UFUNCTION(BlueprintCallable)
	void SetPauseMenuActivated(bool activated);
	bool GetPauseMenuActivated() const;
	void SetClassification(TArray<FString> stringArray);
	UFUNCTION(BlueprintPure)
	TArray<URaceComponent*> GetClassification();
	int32 GetNextObjectID();
	void AddObjectInGame(AObjectBase* ObjectBase);
	void RemoveObjectInGame(AObjectBase* ObjectBase);
	AObjectBase* GetObjectInGame(FString username, int32 id);
	void Teleport(ACheckPoint* checkPoint, AVehiclePawn* vehicle,const FVector& offset);
};


USTRUCT()
struct TFG_SOURCECODE_API FObjectProbabilityStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	TArray<float> probabilities;
};
