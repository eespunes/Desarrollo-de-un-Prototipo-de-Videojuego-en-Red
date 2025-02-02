// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

#include "ClassificationWidget.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UClassificationWidget : public UUserWidget
{
	GENERATED_BODY()


private:
	URaceGameInstance* gameInstance;

	UFUNCTION(BlueprintPure)
	TArray<URaceComponent*> GetClassification();
	UFUNCTION(BlueprintCallable)
	void GetRewards();
};

USTRUCT()
struct TFG_SOURCECODE_API FRewardStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString username;
	UPROPERTY()
	int32 value;
};
