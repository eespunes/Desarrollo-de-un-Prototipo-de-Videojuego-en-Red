// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RaceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API URaceGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Difficulty")
	float lowerDifficulty = 1500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Difficulty")
	float mediumDifficulty = 2000.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Difficulty")
	float higherDifficulty = 2500.f;

	int currentDifficulty = -1;

public:
	float GetDifficulty() const;
	UFUNCTION(BlueprintCallable)
	void SetCurrentDifficulty(int CurrentDifficulty);
};
