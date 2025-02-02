// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIVehicleController.generated.h"

class AVehiclePawn;
class State;
class ACheckPoint;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AAIVehicleController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="AI")
	float acceptableDistance;
	TArray<ACheckPoint*> checkpoints;
	State* currentState;
	AVehiclePawn* vehicle;
	int checkpointIndex;

	void FindCheckpoints();

public:
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	~AAIVehicleController();

	AVehiclePawn* GetVehicle() const;
	void SetCurrentState(State* CurrentState);
	ACheckPoint* GetCurrentCheckpoint();
protected:
	virtual void BeginPlay() override;
};
