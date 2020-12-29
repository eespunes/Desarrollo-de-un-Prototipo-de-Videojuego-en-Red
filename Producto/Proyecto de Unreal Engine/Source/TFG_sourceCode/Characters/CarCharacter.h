// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleCharacter.h"
#include "CarCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ACarCharacter : public AVehicleCharacter
{
	GENERATED_BODY()
public:
	ACarCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void Accelerate() override;

protected:
	virtual void BeginPlay() override;

private:
};
