// Fill out your copyright notice in the Description page of Project Settings.


#include "TyreComponent.h"

#include <dshow.h>
#include <windows/PxWindowsIntrinsics.h>


#include "DrawDebugHelpers.h"
#include "MovieSceneSequenceID.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

// Sets default values for this component's properties
UTyreComponent::UTyreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}

// Called when the game starts
void UTyreComponent::BeginPlay()
{
	Super::BeginPlay();

	AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(GetOwner());
	if (vehiclePawn)
		vehicleMesh = vehiclePawn->GetMesh();
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}
}

bool UTyreComponent::SuspensionForce(float suspensionDistance, float force, float frictionValue, float* TerrainFriction)
{
	if (!vehicleMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance"));
		return false;
	}

	FHitResult hit;
	FVector position = GetComponentLocation();
	FVector end = position - (GetUpVector() * suspensionDistance);

	GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);

	if (hit.bBlockingHit)
	{
		float currentSuspensionCompression = 1 - (hit.Distance / suspensionDistance);
		float springForce = currentSuspensionCompression * force;
		float frictionForce = (currentSuspensionCompression - suspensionCompression);
		frictionForce /= GetWorld()->DeltaTimeSeconds;
		frictionForce *= frictionValue;
		springForce += frictionForce;

		vehicleMesh->AddForceAtLocation(hit.Normal * springForce, position, NAME_None);
		impactPoint = hit.ImpactPoint;
		rootPoint->SetWorldLocation(impactPoint);

		suspensionCompression = currentSuspensionCompression;

		if (hit.Actor->ActorHasTag(TEXT("Grass")))
		{
			*TerrainFriction = 0.5f;
		}

		return true;
	}

	else
	{
		suspensionCompression = 0;
		rootPoint->SetWorldLocation(end);
		impactPoint = FVector::ZeroVector;
		DrawDebugLine(GetWorld(), position, end, FColor::Red, false, -1, 0,10);
		return false;
	}
}

void UTyreComponent::RotateTyres(float currentVelocity)
{
	mesh->AddLocalRotation(FRotator(-currentVelocity * GetWorld()->DeltaTimeSeconds / 2, 0, 0));
}

void UTyreComponent::Steer(float value)
{
	RotateYawTyre(value, 15);
}

void UTyreComponent::Drift(float value)
{
	RotateYawTyre(-value, 30);
}

void UTyreComponent::RotateYawTyre(float value, int32 maxRotation)
{
	if (value == 0)
	{
		rootPoint->SetRelativeRotation(FRotator::ZeroRotator);
		return;
	}
	if (FMath::Sign(rootPoint->GetRelativeRotation().Yaw) != FMath::Sign(value) || FMath::Abs(
		rootPoint->GetRelativeRotation().Yaw) < maxRotation)
	{
		rootPoint->AddRelativeRotation(FRotator(0, value, 0));
	}
}

void UTyreComponent::SetRootPoint(USceneComponent* RootPoint)
{
	rootPoint = RootPoint;
}

void UTyreComponent::SetMesh(UStaticMeshComponent* Mesh)
{
	mesh = Mesh;
}

USceneComponent* UTyreComponent::GetRootPoint() const
{
	return rootPoint;
}

FVector UTyreComponent::GetImpactPoint() const
{
	return impactPoint;
}
