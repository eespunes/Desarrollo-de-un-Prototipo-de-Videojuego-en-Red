// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TFG_SourceCode/Characters/IVehicle.h"
#include "TFG_SourceCode/Characters/VehicleCharacter.h"


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
protected:
	virtual void Accelerate() override;
	virtual void BeginPlay() override;
private:
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	// USpringArmComponent* springArm;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	// UCameraComponent* camera;

	AVehicleCharacter* vehicleCharacter;

	virtual void SetupInputComponent() override;
};
