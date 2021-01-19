// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include "DrawDebugHelpers.h"
#include "Math/UnitConversion.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	RootComponent = mesh;

	// springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	// springArm->SetupAttachment(chassisMesh);
	//
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
	acceleration = (maxSpeed / accelerationRate) * 100;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// if (inGround)
	// 	chassisMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * 0);
	GravityForce();
	SuspensionForces();
	CalculateSpeed(mesh->GetPhysicsLinearVelocity());
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
	float angular = (mesh->GetPhysicsAngularVelocity() * GetActorUpVector()).Size();
	// if (isDrifting)
	// {
	// 	chassisMesh->SetPhysicsMaxAngularVelocityInDegrees(maxDriftAngle);
	// 	if (driftValue == 0)
	// 	{
	// 		if (value != 0)
	// 			driftValue = maxDriftAngle * value;
	// 		else
	// 			isDrifting = false;
	// 	}
	//
	// 	driftValue += GetWorld()->DeltaTimeSeconds * value * 10;
	//
	// 	chassisMesh->SetPhysicsAngularVelocityInDegrees(
	// 		FVector(
	// 			chassisMesh->GetPhysicsAngularVelocity().X,
	// 			chassisMesh->GetPhysicsAngularVelocity().Y,
	// 			driftValue));
	// }
	// else
	// {
	// driftValue = 0;

	mesh->SetPhysicsMaxAngularVelocityInDegrees(maxTurnAngle);

	if (FMath::Abs(value) == 0)
	{
		if (angular < .25f)
			lastTurnValue = 0;
		mesh->AddTorqueInDegrees(GetActorUpVector() * lastTurnValue * angular * -1, NAME_None, true);
	}
	else
	{
		mesh->AddTorqueInDegrees(GetActorUpVector() * minTurnSpeed * value, NAME_None, true);
		lastTurnValue = value > 0 ? 10 : value < 0 ? -10 : 0;
		// chassisMesh->AddForceAtLocation(
		// 	chassisMesh->GetPhysicsLinearVelocity().RotateAngleAxis(90, GetActorUpVector()) * acceleration,
		// 	GetCenterOfMass());
	}
	DrawDebugLine(GetWorld(), GetActorLocation(),
              GetActorLocation() * mesh->GetPhysicsLinearVelocity().Normalize() * 100, FColor::Green,
              false, -1, 0, 5);
	// if (FMath::Abs(value) >= 1)
	// 	turnTimer += GetWorld()->DeltaTimeSeconds;
	// else
	// 	turnTimer = 0;
	// if (turnTimer >= 1)isDrifting = true;
	// }
	// if (turnTimer >= 1 && isDrifting && value == 0)isDrifting = false;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
			                                 TEXT("Angular: %f"), angular));
	}
}

void AVehiclePawn::Drift()
{
	isDrifting = !isDrifting;
	turnTimer = 0;
}

float AVehiclePawn::CalculateRotation(float value) const
{
	float percentage = FMath::Abs(currentSpeed) / maxSpeed;
	if (percentage > frictionDecelerationRate)
		percentage = 1;
	else
		percentage /= frictionDecelerationRate;

	return value * (isDrifting ? maxDriftAngle : maxTurnAngle * percentage);
}

FVector AVehiclePawn::GetCenterOfMass()
{
	return mesh->GetCenterOfMass() - GetActorUpVector() * 10 +
		GetActorForwardVector() * 10;
}

void AVehiclePawn::CalculateSpeed(FVector additionalForces)
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float currentVelocity = (GetActorForwardVector() * mesh->GetPhysicsLinearVelocity()).Size();

	FString action;
	if (inGround)
		if (isAccelerating & !isBraking)
		{
			mesh->SetLinearDamping(0.1f);
			if (currentVelocity < maxSpeed)
			{
				mesh->AddForceAtLocation(GetActorForwardVector() * acceleration, GetCenterOfMass());
				lastVelocity = currentVelocity;
			}
			action = TEXT("Accelerating");
		}
		else if (!isAccelerating & isBraking)
		{
			mesh->SetLinearDamping(0.1f);
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
	// if (inGround)
	// 	chassisMesh->SetPhysicsLinearVelocity((GetActorForwardVector() * currentSpeed) + additionalForces*10);
	if (GEngine)
	{
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

void AVehiclePawn::SuspensionForces()
{
}

float AVehiclePawn::GetSpeed() const
{
	return currentSpeed;
}

UStaticMeshComponent* AVehiclePawn::GetMesh() const
{
	return mesh;
}

URaceComponent* AVehiclePawn::GetRaceComponent() const
{
	return raceComponent;
}
