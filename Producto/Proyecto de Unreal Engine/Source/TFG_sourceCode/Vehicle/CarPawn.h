// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehiclePawn.h"
#include "WheelComponent.h"

#include "CarPawn.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ACarPawn : public AVehiclePawn
{
	GENERATED_BODY()
public:
	ACarPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void Accelerate() override;
	virtual void Brake() override;
	virtual void Turn(float value) override;
	virtual void Drift() override;

protected:
	virtual void BeginPlay() override;
	virtual void SuspensionForces() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UWheelComponent* rightFrontWheel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* rightFrontWheelMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UWheelComponent* leftFrontWheel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* leftFrontWheelMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UWheelComponent* rightRearWheel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* rightRearWheelMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UWheelComponent* leftRearWheel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* leftRearWheelMesh;
};
