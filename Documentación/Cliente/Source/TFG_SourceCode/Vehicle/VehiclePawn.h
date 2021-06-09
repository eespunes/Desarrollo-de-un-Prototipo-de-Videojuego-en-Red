// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <stdbool.h>

#include "IVehicle.h"
#include "Components/NetworkComponent.h"
#include "Components/TextRenderComponent.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

#include "VehiclePawn.generated.h"

class USpringArmComponent;
class UNetworkComponent;
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
	virtual void Drift() override;
	virtual void UseObject() override;
	virtual void Teleport() override;
	
	FVector GetForward();
	UStaticMeshComponent* GetMesh() const;
	UFUNCTION(BlueprintPure)
	URaceComponent* GetRaceComponent() const;
	UFUNCTION(BlueprintPure)
	UNetworkComponent* GetNetworkComponent() const;

	AObjectBase* GetCurrentObject() const;
	void SetCurrentObject(TSubclassOf<UObject> CurrentObject, int32 id = -1);
	void RemoveObject();

	float GetSpeed();
	void SetSpeed(float speedField);
	float GetMaxSpeed() const;
	void SetMaxSpeedMultiplier(float multiplier);
	void ResetMaxSpeedMultiplier();

	void Damage();
	void InstantiateParticle(const TSubclassOf<AActor>& particle);
	void InvertControls();
	void NormalControls();

	bool GetHasBeenHit() const;

	bool GetIsAccelerating() const;
	void SetIsAccelerating(bool bIsAccelerating);
	bool GetIsBraking();
	void SetIsBraking(bool bIsBraking);
	void SetSteerValue(float bSteerValue);
	bool GetIsDrifting() const;
	void SetIsDrifting(bool bIsDrifting);

	float GetDriftValue();
	void SetDriftValue(float driftValueField);
	float GetAccelerationTimer();
	void SetAccelerationTimer(float accelerationTimerField);
	float GetBrakeTimer();
	void SetBrakeTimer(float brakeTimerField);
	float GetDecelerationTimer();
	void SetDecelerationTimer(float decelerationTimerField);
	float GetDriftIncreaseTimer();
	void SetDriftIncreaseTimer(float driftIncreaseTimerField);
	float GetDriftDecreaseTimer();
	void SetDriftDecreaseIncreaseTimer(double driftDecreaseTimerField);

	float GetDriftSign();
	UTextRenderComponent* GetPlayerText();
	void SetHasBeenHit(bool hit);
	void ResetForces(FVector newForward);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* carMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* objectSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* particleSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* normalSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* normalCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTextRenderComponent* playerNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URaceComponent* raceComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UNetworkComponent* networkComponent;

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
	float maxSpeed = 500.f;
	float initialMaxSpeed;
	float maxSpeedMultiplier = 1;
	float speed;
	bool isAccelerating = false;
	float accelerationTimer;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float accelerationRate = 1.f;

	//Brake
	bool isBraking;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float brakeRate = 1.5f;
	float brakeTimer;

	//Reverse
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float reverseRate = 3;
	float reverseSpeed;

	//Deceleration
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float decelerationRate;
	float decelerationTimer;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float flyRate;

	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	int maxPitchRotation = 5;

	//Turn
	UPROPERTY(EditAnywhere, Category="Vehicle: Turn")
	float steeringRate = 40.f;
	float lastTurnValue;
	float steerValue;

	//Drift
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float finalDriftingRate = 100.f;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float initialDriftingRate = 20.f;
	bool isDrifting;
	float driftValue;
	float driftIncreaseTimer;
	float driftDecreaseTimer;
	float driftSign;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float driftRateIncrease = 20;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift")
	float driftRateDecrease = 40;
	float cameraRotation = 15;
	float driftCameraRotationValue;
	bool traction4x4 = false;

	//Drift Boost
	bool canDriftBoost;
	bool performDriftBoost;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift Boost")
	float driftBoostRate = 1.25f;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift Boost")
	float driftBoostTime = 1;
	float driftBoostTimer;
	UPROPERTY(EditAnywhere, Category="Vehicle: Drift Boost")
	TSubclassOf<class AActor> driftBoostParticle;
	TArray<AActor*> boostParticles;

	//Objects
	AObjectBase* currentObject = nullptr;
	UPROPERTY(EditAnywhere, Category="Vehicle: Object")
	float hitWaiting = 3;
	bool hasBeenHit = true;
	float hitTimer;
	bool invertControls;
	AActor* currentParticle;
	UPROPERTY(EditAnywhere, Category="Vehicle: Object")
	TSubclassOf<UMatineeCameraShake> cameraShake;

	URaceGameInstance* gameInstance;

	bool performObjectBoost;

	UPROPERTY(EditAnywhere, Category="Vehicle: Camera")
	int constantFieldOfView;
	UPROPERTY(EditAnywhere, Category="Vehicle: Camera")
	int variableFieldOfView;
	bool inForward;
	bool inReverse;
	FVector forward;
	bool resetForces;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement();

	void PerformAcceleration();
	void PerformBraking();
	void PerformSteering();
	void PerformDrift();

	void StartDrift();
	void StopDrift();

	void GravityForce();
	virtual void SuspensionForces();
	virtual void InstantiateDriftBoostParticles();

	void WaitAfterHit(float DeltaTime);

	FVector CalculateAverageDirection(TArray<FVector> vectors);
};
