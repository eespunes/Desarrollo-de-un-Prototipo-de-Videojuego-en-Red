// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IVehicle.h"
#include "VehiclePawn.generated.h"

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
	virtual void Turn(float value) override;
	virtual void Drift() override;
	virtual void UseObject() override;
	void RemoveObject();

	UStaticMeshComponent* GetMesh() const;
	UFUNCTION(BlueprintPure)
	URaceComponent* GetRaceComponent() const;

	AObjectBase* GetCurrentObject() const;

	void SetCurrentObject(AObjectBase* CurrentObject);
	float GetMaxSpeed() const;
	void SetMaxSpeed(float speed);
	float GetInitialMaxSpeed();
	void Damage();
	void InstantiateParticle(const TSubclassOf<AActor>& particle);
	void InvertControls();
	void NormalControls();
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USceneComponent* objectSpawnPoint;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USceneComponent* particleSpawnPoint;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	URaceComponent* raceComponent;

	UPROPERTY(EditAnywhere,Category="Vehicle: Suspension")
	float suspensionDistance = 100.f;
	UPROPERTY(EditAnywhere,Category="Vehicle: Suspension")
	float suspensionRate;
	UPROPERTY(EditAnywhere,Category="Vehicle: Suspension")
	float dampingRate;

	FVector lastUpVector;
	bool inGround;

	//Accelerate
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float accelerationRate = 1.f;
	float acceleration;
	UPROPERTY(EditAnywhere,Category="Vehicle: Speed")
	float maxSpeed = 500.f;
	float initialMaxSpeed;
	bool isAccelerating = false;
	float lastVelocity;
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
	float steeringRate = 10.f;
	UPROPERTY(EditAnywhere,Category="Vehicle: Turn")
	float maxTurnAngle = 50.f;
	UPROPERTY(EditAnywhere,Category="Vehicle: Turn")
	int turnToDriftSeconds;
	float turnTimer;
	float lastTurnValue;
	float turnValue;

	//Drift
	UPROPERTY(EditAnywhere,Category="Vehicle: Drift")
	float maxDriftAngle = 100.f;
	bool isDrifting;
	float driftValue;
	float driftTimer;
	float driftSign;
	float lastAngular;

	//Objects
	AObjectBase* currentObject = nullptr;
	float hitWaiting = 3;
	bool canUseObject{true};
	float hiTimer;
	bool invertControls;
	AActor* currentParticle;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement();
	float CalculateMaxDriftValue();
	FVector GetCenterOfMass() const;
	void GravityForce() const;
	virtual void SuspensionForces();
};
