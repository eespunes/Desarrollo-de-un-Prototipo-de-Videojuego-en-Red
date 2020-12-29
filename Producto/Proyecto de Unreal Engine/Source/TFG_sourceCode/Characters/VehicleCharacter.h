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

protected:
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere,Category="Speed")
	float acceleration;
	UPROPERTY(EditAnywhere,Category="Speed")
	float maxSpeed;
	float currentSpeed = 0.f;
	float isAccelerating = false;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
};
