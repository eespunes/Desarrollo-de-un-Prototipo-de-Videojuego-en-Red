// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <stdbool.h>

#include "IVehicle.h"
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
	void StopDrift();
	virtual void UseObject() override;
	void RemoveObject();

	FVector GetForward() const;
	FVector GetUp() const;
	UStaticMeshComponent* GetMesh() const;
	UFUNCTION(BlueprintPure)
	URaceComponent* GetRaceComponent() const;
	UNetworkComponent* GetNetworkComponent() const;
	AObjectBase* GetCurrentObject() const;
	void SetCurrentObject(TSubclassOf<UObject> CurrentObject);
	float GetMaxSpeed() const;
	void SetMaxSpeed(float speed);
	float GetInitialMaxSpeed() const;
	void Damage();
	void InstantiateParticle(const TSubclassOf<AActor>& particle);
	void InvertControls();
	void NormalControls();
	bool GetHasBeenHit() const;
	bool GetIsAccelerating() const;
	void SetIsAccelerating(bool bIsAccelerating);
	void SetIsBraking(bool bIsBraking);
	void SetSteerValue(float bSteerValue);
	void SetIsDrifting(bool bIsDrifting);
	FVector GetCenterOfMass() const;
	bool GetDrifting() const;
	bool GetBraking();
	float GetCurrentSpeed();
	float GetDriftSign();
	UTextRenderComponent* GetPlayerText();
	void SetPing(int32 ping);
	UFUNCTION(BlueprintPure)
	int32 GetPing();

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
	float accelerationRate = 1.f;
	float acceleration;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float maxSpeed = 500.f;
	float initialMaxSpeed;
	float currentSpeed;
	bool isAccelerating = false;
	float lastVelocity;
	float accelerationTimer;

	//Brake
	bool isBraking;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float brakeRate = 1.5f;
	float brakeTimer;

	//Reverse
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float reverseRate = 3;
	float reverseSpeed;
	float reverseAcceleration;
	float reverseTimer;

	//Deceleration
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float frictionDecelerationRate;
	UPROPERTY(EditAnywhere, Category="Vehicle: Speed")
	float frictionRate;
	float decelerationTimer;

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
	float hitWaiting = 3;
	bool hasBeenHit{true};
	float hitTimer;
	bool invertControls;
	AActor* currentParticle;

	float cameraRotation = 15;
	float driftCameraRotationValue;
	float cameraRotationConstant = 0;
	bool traction4x4 = false;

	URaceGameInstance* gameInstance;
	
	float terrainFriction=1;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement();
	void PerformAcceleration();
	void PerformBraking(float& currentVelocity);
	void PerformSteering(float currentVelocity, float currentAngular);
	void PerformDrift(float currentVelocity, float currentAngular);
	float CalculateMaxSteerValue(float currentVelocity);
	float CalculateMaxDriftValue(float currentVelocity);

	void GravityForce() const;
	virtual void SuspensionForces();
	virtual void InstantiateDriftBoostParticles();

	void WaitAfterHit(float DeltaTime);
};
