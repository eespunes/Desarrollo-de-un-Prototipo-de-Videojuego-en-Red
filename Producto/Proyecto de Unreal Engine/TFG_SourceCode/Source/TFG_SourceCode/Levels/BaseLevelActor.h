// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ILevel.h"
#include "Chaos/AABB.h"
#include "Engine/LevelScriptActor.h"


#include "BaseLevelActor.generated.h"

class URaceComponent;
class ACheckPoint;

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ABaseLevelActor : public ALevelScriptActor, public ILevel
{
private:

	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	int32 numberOfLaps;
	int32 numberOfCheckpoints;
	
	TArray<URaceComponent*> vehicles;
	TArray<ACheckPoint*> checkpoints;
	void FindCheckpoints();
	void FindVehicles();
	virtual void BeginPlay() override;
public:
	ABaseLevelActor();
	int32 GetNumberOfLaps() const;
	int32 GetNumberOfCheckpoints() const;
	ACheckPoint* GetCheckpoint(int32 idx);
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
};
