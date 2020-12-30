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
	virtual void Accelerate() override;
	virtual void Brake() override;
	virtual void Turn(float value) override;
protected:
	virtual void BeginPlay() override;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	// USpringArmComponent* springArm;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	// UCameraComponent* camera;

private:
	AVehicleCharacter* vehicleCharacter;
	//HUD
	UPROPERTY(EditAnywhere,Category="HUD")
	TSubclassOf<class UUserWidget> hudClass;

	virtual void SetupInputComponent() override;
	void Actions();
	void Axis();
};
