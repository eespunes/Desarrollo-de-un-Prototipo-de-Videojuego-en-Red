// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ILevel.h"
#include "Chaos/AABB.h"
#include "Engine/LevelScriptActor.h"
#include "GameFramework/PlayerStart.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"


#include "RacingLevel.generated.h"

class ARaceController;
class AVehiclePawn;
class URaceComponent;
class ACheckPoint;

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ARacingLevel : public ALevelScriptActor, public ILevel
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int32 numberOfLaps;
	ARaceController* raceController;
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<class UUserWidget> classificationClass;
	bool playerHasFinished;

	virtual void BeginPlay() override;
public:
	ARacingLevel();

	int32 GetNumberOfLaps() const;
	int32 GetNumberOfCheckpoints() const;
	int32 GetNumberOfVehicles() const;
	ACheckPoint* GetCheckpoint(int32 idx) const;
	URaceComponent* GetVehicleByPosition(int32 position);
	URaceComponent* GetVehicleByUsername(FString username);
	bool GetCanRace() const;
	void SetCanRace(bool value) const;
	bool GetCanStartRace() const;
	void SetCanStartRace(bool value) const;
	APlayerStart* GetRaceStart(int idx) const;
	void SetPlayerHasFinished();
	UFUNCTION(BlueprintPure)
	bool GetPlayerHasFinished();
	UFUNCTION(BlueprintPure)
	TArray<URaceComponent*> GetVehicles() const;
	void SetClassification(TArray<URaceComponent*> classification);
};
