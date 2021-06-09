// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"
#include "TyreComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFG_SOURCECODE_API UTyreComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTyreComponent();
	bool SuspensionForce(float suspensionDistance, float force, float frictionValue,float* TerrainFriction);
	void RotateTyres(float currentVelocity);
	void Steer(float value);
	void Drift(float value);

	void RotateYawTyre(float value, int32 maxRotation);

	void SetRootPoint(USceneComponent* RootPoint);
	void SetMesh(UStaticMeshComponent* Mesh);

	USceneComponent* GetRootPoint() const;
	FVector GetImpactPoint() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	USceneComponent* rootPoint;
	UStaticMeshComponent* mesh;
	float suspensionCompression;
	UStaticMeshComponent* vehicleMesh;
	float rotationValue;
	float initialRotation = 0.5f;
	float lastRotationValue;
	float maxTyreRotation = 15;
	float currentTyreRotation;
	FVector impactPoint;
};
