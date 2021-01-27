// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include <iterator>


#include "DrawDebugHelpers.h"
#include "Math/UnitConversion.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	RootComponent = mesh;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(mesh);

	raceComponent = CreateDefaultSubobject<URaceComponent>(TEXT("Race Component"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / 3;
	acceleration = (maxSpeed / accelerationRate) * 200;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GravityForce();
	SuspensionForces();
	Movement();
}

void AVehiclePawn::Accelerate()
{
	isAccelerating = !isAccelerating;
}

void AVehiclePawn::Brake()
{
	isBraking = !isBraking;
}

void AVehiclePawn::Turn(float value)
{
	turnValue = value;
}

void AVehiclePawn::Drift()
{
	if (turnTimer < turnToDriftSeconds)
		isDrifting = !isDrifting;
}

float AVehiclePawn::CalculateMaxDriftValue() const
{
	// if (turnValue == 1)
	// 	driftValue +=turnValue
	return driftValue >= maxDriftAngle ? maxDriftAngle : driftValue;
}

void AVehiclePawn::Movement()
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float currentVelocity = (GetActorForwardVector() * mesh->GetPhysicsLinearVelocity()).Size();
	float angular = (mesh->GetPhysicsAngularVelocityInDegrees() * GetActorUpVector()).Size();

	FString action;
	if (inGround)
	{
		if (isAccelerating & !isBraking)
		{
			mesh->SetLinearDamping(1.f);
			if (currentVelocity < maxSpeed)
			{
				mesh->AddForceAtLocation(GetActorForwardVector() * acceleration, GetCenterOfMass());
				lastVelocity = currentVelocity;
			}
			action = TEXT("Accelerating");
		}
		else if (!isAccelerating & isBraking)
		{
			mesh->SetLinearDamping(1.f);
			currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
			if (currentVelocity > reverseSpeed)
			{
				mesh->AddForceAtLocation(-GetActorForwardVector() * acceleration * brakeRate, GetCenterOfMass());
				lastVelocity = currentVelocity;
			}

			action = TEXT("Braking");
		}
		else if (isBraking && isAccelerating)
		{
			currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
			mesh->SetLinearDamping(2.5f);
			action = TEXT("Braking and Accelerating");
		}
		else
		{
			mesh->SetLinearDamping(1.f);
			action = TEXT("Nothing");
		}

		if (isDrifting)
		{
			action += TEXT(" - Drift");
			mesh->SetPhysicsMaxAngularVelocityInDegrees(
				CalculateMaxDriftValue());

			if (turnTimer >= turnToDriftSeconds && FMath::Abs(turnValue) <= 0)
				isDrifting = false;

			mesh->AddTorqueInDegrees(GetActorUpVector() * minTurnSpeed * turnValue * currentVelocity / maxSpeed,
			                         NAME_None, true);
		}
		else
		{
			mesh->SetPhysicsMaxAngularVelocityInDegrees(maxTurnAngle);
			if (FMath::Abs(turnValue) == 0 || (currentVelocity / maxSpeed) < frictionDecelerationRate)
			{
				if (angular < .25f)
					lastTurnValue = 0;
				mesh->AddTorqueInDegrees(GetActorUpVector() * lastTurnValue * angular * -1, NAME_None, true);
			}
			else
			{
				action += TEXT(" - Turn");
				if (FMath::Abs(turnValue) >= 0)
					turnTimer += GetWorld()->DeltaTimeSeconds;
				else
				{
					turnTimer = 0;
				}
				if (turnTimer >= turnToDriftSeconds) { isDrifting = true; }
				else
				{
					mesh->AddTorqueInDegrees(GetActorUpVector() * minTurnSpeed * turnValue * currentVelocity / maxSpeed,
					                         NAME_None, true);
					lastTurnValue = turnValue > 0 ? 10 : turnValue < 0 ? -10 : 0;
				}
			}
		}
	}

	//DEBUG
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
			                                 TEXT("Angular: %f"), angular));
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Speed: %f"), currentVelocity));
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue, FString::Printf(TEXT("%s"), *action));
	}
}

void AVehiclePawn::GravityForce() const
{
	mesh->AddForce(-GetActorUpVector() * 980, NAME_None, true);
}


FVector AVehiclePawn::GetCenterOfMass() const
{
	return mesh->GetCenterOfMass() - GetActorUpVector() * 10 +
		GetActorForwardVector() * 10;
}

void AVehiclePawn::SuspensionForces()
{
}

UStaticMeshComponent* AVehiclePawn::GetMesh() const
{
	return mesh;
}

URaceComponent* AVehiclePawn::GetRaceComponent() const
{
	return raceComponent;
}
