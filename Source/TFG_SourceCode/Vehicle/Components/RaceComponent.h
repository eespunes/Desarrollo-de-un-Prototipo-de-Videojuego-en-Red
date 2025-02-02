// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "NetworkComponent.h"
#include "Components/ActorComponent.h"

#include "RaceComponent.generated.h"

class AVehiclePawn;
class URaceGameInstance;
class ACheckPoint;
class ARacingLevel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFG_SOURCECODE_API URaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URaceComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetCheckpoint();
	UFUNCTION(BlueprintPure)
	int32 GetPosition() const;
	ACheckPoint* GetCurrentCheckPoint() const;
	void Ragnarok(int racePosition);
	int32 GetCheckpointPosition() const;
	void SetCheckpoint(ACheckPoint* newCheckpoint);
	void SetPosition(int32 Position);

	UFUNCTION(BlueprintPure)
	int32 GetCurrentLap() const;
	UFUNCTION(BlueprintPure)
	int32 GetMaxLap() const;
	float GetDistance();
	UFUNCTION(BlueprintPure)
	float GetTimeValue();
	bool InFrontOfOpponent(URaceComponent* opponent);
	UFUNCTION(BlueprintPure)
	FString GetUsername();
	UFUNCTION(BlueprintPure)
	bool HasFinished();
	void Destroy();
	void SetTime(float networkTime);
	UFUNCTION(BlueprintPure)
	static FString GenerateTimeString(float time);
	UFUNCTION(BlueprintPure)
	float GetCurrentLapTime() const;
	UFUNCTION(BlueprintPure)
	float GetBestLapTime() const;
	UFUNCTION(BlueprintPure)
	bool CanRace() const;
	UFUNCTION(BlueprintCallable)
	void SetCanRace(bool value) const;
	UFUNCTION(BlueprintPure)
	bool CanStartRace() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int32 maxCheckpoints;
	ACheckPoint* checkpoint;
	int32 maxLaps;
	int32 currentLap;
	int32 position = -1;
	float currentLapTime;
	float bestLapTime = -1;
	float timeValue;
	float generalLapTime;
	URaceGameInstance* gameInstance;
	AVehiclePawn* vehicle;
	bool finished;
	bool isPlayer;
};
