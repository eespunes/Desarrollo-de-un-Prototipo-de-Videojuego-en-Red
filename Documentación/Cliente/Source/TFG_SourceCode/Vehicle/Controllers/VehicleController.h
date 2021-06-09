// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Vehicle/IVehicle.h"

#include "VehicleController.generated.h"

class AVehiclePawn;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AVehicleController : public APlayerController, public IVehicle
{
	GENERATED_BODY()
public:
	virtual void Accelerate() override;
	virtual void Brake() override;
	virtual void Steer(float value) override;
	virtual void Drift() override;
	virtual void UseObject() override;
	virtual void Teleport() override;
	void Pause();
protected:
	virtual void BeginPlay() override;

private:
	AVehiclePawn* vehiclePawn;
	//HUD
	UPROPERTY(EditAnywhere,Category="UI")
	TSubclassOf<class UUserWidget> hudClass;
	UPROPERTY(EditAnywhere,Category="UI")
	TSubclassOf<class UUserWidget> pauseClass;
	URaceGameInstance* gameInstance;
	bool pauseMenuActivated;

	virtual void SetupInputComponent() override;
	void Actions();
	void Axis();
};
