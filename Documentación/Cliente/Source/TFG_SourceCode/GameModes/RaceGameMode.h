// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RaceGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "TFG_SourceCode/TFG_SourceCodeGameModeBase.h"

#include "RaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ARaceGameMode : public ATFG_SourceCodeGameModeBase
{
	GENERATED_BODY()

public:
	void SingleplayerSpawn(APlayerStart*& FoundPlayerStart);
	void MultiplayerSpawn(APlayerStart*& FoundPlayerStart);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings", meta=(AllowPrivateAccess="true"))
	int32 playerPosition = 12;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AVehiclePawn> vehicleToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AAIVehicleController> singleplayerController;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AVehicleNetworkController> multiplayerController;
	URaceGameInstance* gameInstance;

	void FindStartPoints(TArray<APlayerStart*>& startPoints);
	void OrderStartPoints(TArray<APlayerStart*>& UnOccupiedStartPoints);
};
