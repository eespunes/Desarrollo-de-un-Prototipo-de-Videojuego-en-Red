// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"
#include "WheelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFG_SOURCECODE_API UWheelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWheelComponent();
	bool SuspensionForce(float suspensionDistance, float force, float frictionValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	float suspensionCompression;
	UStaticMeshComponent* vehicleMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* mesh;
};
