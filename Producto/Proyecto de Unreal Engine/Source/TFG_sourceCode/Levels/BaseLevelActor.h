// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ILevel.h"
#include "Chaos/AABB.h"
#include "Engine/LevelScriptActor.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "BaseLevelActor.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ABaseLevelActor : public ALevelScriptActor, public ILevel
{
private:

	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	int numberOfLaps;
	int numberOfCheckpoints;


protected:
	virtual void BeginPlay() override;
public:
	int GetNumberOfLaps() const;
	int GetNumberOfCheckpoints() const;
};
