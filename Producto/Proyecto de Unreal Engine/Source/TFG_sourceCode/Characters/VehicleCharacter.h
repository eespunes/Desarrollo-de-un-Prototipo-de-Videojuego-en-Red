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
	virtual void Turn(float value) override;
	
	UFUNCTION(BlueprintPure)
	float GetSpeed() const;
protected:
	USceneComponent* root;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCapsuleComponent* capsule;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USkeletalMeshComponent* skinnedMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* camera;
	//Accelerate
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float accelerationRate = 1.f;
	float acceleration;
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float maxSpeed = 4000.f;
	float currentSpeed = 0.f;
	bool isAccelerating = false;
	//Brake
	bool isBraking;
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float brakeRate = 1.5f;
	//Reverse
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float reverseRate = 3;
	float reverseSpeed;
	//Not Accelerating, nor braking
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float frictionDecelerationRate;

	//Turn
	UPROPERTY(EditAnywhere,Category="Vehicle: Turn")
	float minTurnSpeed = 10.f;
	UPROPERTY(EditAnywhere,Category="Vehicle: Turn")
	float maxTurnSpeed = 50.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CalculateSpeed(float DeltaTime);
	float CalculateRotation(float DeltaTime) const;
	void FrictionBraking(float DeltaTime);
};
