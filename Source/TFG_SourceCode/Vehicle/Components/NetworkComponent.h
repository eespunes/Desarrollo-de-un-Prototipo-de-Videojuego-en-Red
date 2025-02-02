// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "RaceComponent.h"
#include "Components/ActorComponent.h"

#include "NetworkComponent.generated.h"


struct FRacingDataStruct;
class URaceGameInstance;
class AVehiclePawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFG_SOURCECODE_API UNetworkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNetworkComponent();

protected:
	FString GenerateMessage();
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	URaceGameInstance* gameInstance;
	FString raceID;
	FString username;
	float sendTurnTime = 0.250f;
	float sendTimer = 0;
	int32 messageId;
	bool isPlayer;
	FRacingDataStruct* racingData;
	AVehiclePawn* vehicle;

	int32 ping;
	bool canSend;
	bool notSendedFinish;

	void SendMessage();
	
	void SetDataIsAccelerating();
	void SetDataIsBraking();
	void SetDataTurnValue(float value);
	void SetDataIsDrifting();
	// void SetUseObjectData(bool use);
	void SetUsername(FString other);
	// void SetObjectData(int32 idx);

	bool GetDataIsAccelerating() const;
	bool GetDataIsBraking() const;
	bool GetDataIsDrifting() const;
	float GetDataTurnValue() const;

	UFUNCTION(BlueprintPure)
	int32 GetPing() const;
	void SetPing(int32 Ping);
	FString GetUsername();
};

USTRUCT()
struct TFG_SOURCECODE_API FRacingStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString race;
	UPROPERTY()
	FString username;

	UPROPERTY()
	FString data;
};

USTRUCT()
struct TFG_SOURCECODE_API FRacingDataStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32 id;
	UPROPERTY()
	FString username;
	UPROPERTY()
	bool finished;

	UPROPERTY()
	float speed;

	UPROPERTY()
	bool isAccelerating = false;
	UPROPERTY()
	bool isBraking = false;
	UPROPERTY()
	float turnValue;
	UPROPERTY()
	bool isDrifting;
	UPROPERTY()
	float driftValue;

	UPROPERTY()
	float accelerationTimer;
	UPROPERTY()
	float brakingTimer;
	UPROPERTY()
	float decelerationTimer;
	UPROPERTY()
	float driftIncreaseTimer;
	UPROPERTY()
	float driftDecreaseTimer;

	UPROPERTY()
	float time;
	UPROPERTY()
	int32 lap;
	UPROPERTY()
	int32 checkpoint;
	UPROPERTY()
	float distance;

	UPROPERTY()
	float positionX;
	UPROPERTY()
	float positionY;
	UPROPERTY()
	float positionZ;

	UPROPERTY()
	float rotationRoll;
	UPROPERTY()
	float rotationPitch;
	UPROPERTY()
	float rotationYaw;

	UPROPERTY()
	bool hit;
};
