// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <stdbool.h>

#include "IVehicle.h"
#include "VehiclePawn.generated.h"

class USpringArmComponent;
class UTyreComponent;
class UCameraComponent;
class URaceComponent;
class AObjectBase;
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
	virtual void Steer(float value) override;
	float CalculateMaxSteerValue(float currentVelocity);
	virtual void Drift() override;
	virtual void UseObject() override;
	void RemoveObject();

	FVector GetForward() const;
	UStaticMeshComponent* GetMesh() const;
	UFUNCTION(BlueprintPure)
	URaceComponent* GetRaceComponent() const;
	AObjectBase* GetCurrentObject() const;
	void SetCurrentObject(AObjectBase* CurrentObject);
	float GetMaxSpeed() const;
	void SetMaxSpeed(float speed);
	float GetInitialMaxSpeed() const;
	void Damage();
	void InstantiateParticle(const TSubclassOf<AActor>& particle);
	void InvertControls();
	void NormalControls();
	bool GetHasBeenHit() const;
	bool GetIsAccelerating() const;
	FVector GetCenterOfMass() const;
	bool GetDrifting() const;
	bool GetBraking();
	float GetCurrentSpeed();
	float GetDriftSign();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* carMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* objectSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* particleSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* normalCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* reverseCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URaceComponent* raceComponent;

	UPROPERTY(EditAnywhere, Category="Vehicle: Suspension")
	float suspensionDistance = 100.f;
	UPROPERTY(EditAnywhere, Category="Vehicle: Suspension")
	float suspensionRate;
	UPROPERTY(EditAnywhere, Category="Vehicle: Suspension")
	float dampingRate;
	TArray<UTyreComponent*> tyres;

	FVector lastUpVector;
	bool inGround;

	//Accelerate
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float accelerationRate = 1.f;
	float acceleration;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float maxSpeed = 500.f;
	float initialMaxSpeed;
	bool isAccelerating = false;
	float lastVelocity;
	//Brake
	bool isBraking;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float brakeRate = 1.5f;
	//Reverse
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float reverseRate = 3;
	float reverseSpeed;
	float reverseAcceleration;
	//Not Accelerating, nor braking
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float frictionDecelerationRate;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float frictionRate;

	//Turn
	UPROPERTY(EditAnywhere, Category="Vehicle: Turn")
	float steeringRate = 10.f;
	UPROPERTY(EditAnywhere, Category="Vehicle: Turn")
	float maxSteerAngle = 50.f;
	UPROPERTY(EditAnywhere, Category="Vehicle: Turn")
	int turnToDriftSeconds;
	float turnTimer;
	float lastTurnValue;
	float steerValue;

	//Drift
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float maxDriftAngle = 100.f;
	bool isDrifting;
	float driftValue;
	float driftTimer;
	float driftInverseTimer;
	float driftSign;
	float lastAngular;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float driftRateIncrease = 20;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float driftRateDecrease = 40;

	//Objects
	AObjectBase* currentObject = nullptr;
	float hitWaiting = 3;
	bool hasBeenHit{true};
	float hitTimer;
	bool invertControls;
	AActor* currentParticle;
	float currentSpeed;
	float cameraRotation = 15;
	float cameraRotationConstant = 0;
	float accelerationTimer;
	float reverseTimer;
	float brakeTimer;
	float deaccelerationTimer;
	bool Traction4x4 = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement();
	void PerformAcceleration();
	void PerformBraking(float& currentVelocity);
	void PerformSteering(float currentVelocity, float currentAngular);
	void PerformDrift(float currentVelocity);
	float CalculateMaxDriftValue(float currentVelocity);

	void GravityForce() const;
	virtual void SuspensionForces();

	void WaitAfterHit(float DeltaTime);
};