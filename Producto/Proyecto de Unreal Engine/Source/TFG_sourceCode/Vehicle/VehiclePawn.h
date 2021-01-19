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
#include "TFG_SourceCode/RaceControllers/RaceComponent.h"

#include "VehiclePawn.generated.h"

UCLASS()
class TFG_SOURCECODE_API AVehiclePawn : public APawn, public IVehicle
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehiclePawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Accelerate() override;
	virtual void Brake() override;
	virtual void Turn(float value) override;
	virtual void Drift() override;

	UFUNCTION(BlueprintPure)
	float GetSpeed() const;
	UStaticMeshComponent* GetMesh() const;
	URaceComponent* GetRaceComponent() const;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UStaticMeshComponent* mesh;
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	// USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	URaceComponent* raceComponent;

	// UStaticMeshComponent* wheels[];

	UPROPERTY(EditAnywhere,Category="Vehicle: Suspension")
	float suspensionDistance = 100.f;
	UPROPERTY(EditAnywhere,Category="Vehicle: Suspension")
	float suspensionRate;
	UPROPERTY(EditAnywhere,Category="Vehicle: Suspension")
	float dampingRate;

	FVector lastUpVector;

	//Accelerate
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float accelerationRate = 1.f;
	float acceleration;
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float maxSpeed = 500.f;
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
	float maxTurnAngle = 50.f;
	float lastVelocity;

	UPROPERTY(EditAnywhere,Category="Vehicle: Drift")
	float maxDriftAngle = 100.f;
	int isDrifting;

	bool inGround;
	float turnTimer;
	float driftValue;
	float lastTurnValue;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CalculateSpeed(FVector additionalForces);
	float CalculateRotation(float value) const;
	FVector GetCenterOfMass();

	void GravityForce() const;
	virtual void SuspensionForces();
};
