// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IVehicle.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "VehicleCharacter.generated.h"

UCLASS()
class TFG_SOURCECODE_API AVehicleCharacter : public ACharacter, public IVehicle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicleCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Accelerate() override;
	virtual void Brake() override;
protected:
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere,Category="Speed")
	float accelerationRate = 1.f;
	float acceleration;
	UPROPERTY(EditAnywhere,Category="Speed")
	float maxSpeed = 4000.f;
	float currentSpeed = 0.f;
	bool isAccelerating = false;

	bool isBraking;
	UPROPERTY(EditAnywhere,Category="Speed")
	float brakeRate = 1.5f;
	UPROPERTY(EditAnywhere,Category="Speed")
	float reverseRate = 3;
	float reverseSpeed;
	UPROPERTY(EditAnywhere,Category="Speed")
	float frictionDecelerationRate;
	float frictionDeceleration;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
};
