// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

#include "RaceController.generated.h"

class URaceComponent;
class ACheckPoint;
UCLASS()
class TFG_SOURCECODE_API ARaceController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARaceController();

protected:
	TArray<ACheckPoint*> checkpoints;
	TArray<URaceComponent*> vehicles;

	bool canRace = false;
	TArray<APlayerStart*> raceStarts;
	bool canStartRace;
	URaceGameInstance* gameInstance;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void FindVehicles();
	void FindCheckpoints();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ACheckPoint* GetCheckpoint(int32 idx);
	int GetNumberOfCheckpoints() const;
	int32 GetNumberOfVehicles() const;
	URaceComponent* GetVehicleByPosition(int32 position);
	bool GetCanRace() const;
	void SetCanRace(bool value);
	bool GetCanStartRace() const;
	void SetCanStartRace(bool value);
	APlayerStart* GetRaceStart(int idx);
	void UpdatePositions();

	TArray<URaceComponent*> GetVehicles() const;
	void SetClassification(TArray<URaceComponent*> classification);
	URaceComponent* GetVehicleByUsername(const FString& Username);
};
