// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IVehicle.h"
#include "VehiclePawn.h"


#include "VehicleController.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AVehicleController : public APlayerController, public IVehicle
{
	GENERATED_BODY()
public:
	AVehicleController();
	virtual void Accelerate() override;
	virtual void Brake() override;
	virtual void Turn(float value) override;
	virtual void Drift() override;
protected:
	virtual void BeginPlay() override;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	// USpringArmComponent* springArm;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	// UCameraComponent* camera;

private:
	AVehiclePawn* vehiclePawn;
	//HUD
	UPROPERTY(EditAnywhere,Category="HUD")
	TSubclassOf<class UUserWidget> hudClass;

	virtual void SetupInputComponent() override;
	void Actions();
	void Axis();
};
