// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include "DrawDebugHelpers.h"
#include "Math/UnitConversion.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chassisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	RootComponent = chassisMesh;

	// springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	// springArm->SetupAttachment(chassisMesh);
	//
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(chassisMesh);

	raceComponent = CreateDefaultSubobject<URaceComponent>(TEXT("Race Component"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / 3;
	acceleration = maxSpeed / accelerationRate;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (inGround)
		chassisMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * 0);
	GravityForce();
	SuspensionForces();
	GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
		                                 TEXT("Linear: %f,%f,%f"), GetActorForwardVector().X,
		                                 GetActorForwardVector().Y,
		                                 GetActorForwardVector().Z));
	CalculateSpeed(chassisMesh->GetPhysicsLinearVelocity());
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
	float angular = FMath::Abs(chassisMesh->GetPhysicsAngularVelocity().Z);
	if (isDrifting)
	{
		if (driftValue == 0)
		{
			if (value != 0)
				driftValue = maxDriftAngle * value;
			else
				isDrifting = false;
		}

		driftValue += GetWorld()->DeltaTimeSeconds * value * 10;

		chassisMesh->SetPhysicsAngularVelocityInDegrees(
			FVector(
				chassisMesh->GetPhysicsAngularVelocity().X,
				chassisMesh->GetPhysicsAngularVelocity().Y,
				driftValue));
	}
	else
	{
		driftValue = 0;
		chassisMesh->SetPhysicsAngularVelocityInDegrees(
			FVector(
				chassisMesh->GetPhysicsAngularVelocity().X,
				chassisMesh->GetPhysicsAngularVelocity().Y,
				CalculateRotation(value)));

		if (FMath::Abs(value) >= 1)
			turnTimer += GetWorld()->DeltaTimeSeconds;
		else
			turnTimer = 0;
		if (turnTimer >= 1)isDrifting = true;
	}
	if (turnTimer >= 1 && isDrifting && value == 0)isDrifting = false;
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

void AVehiclePawn::CalculateSpeed(FVector additionalForces)
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float velocitySize = chassisMesh->GetPhysicsLinearVelocity().Size();
	float currentVelocity = FVector(GetActorForwardVector().X * velocitySize,
	                                GetActorForwardVector().Y * velocitySize, 0).Size();
	FString action;

	if (isAccelerating & !isBraking && inGround)
	{
		if (currentSpeed < maxSpeed)
		{
			currentSpeed = FMath::Min(currentSpeed + acceleration * deltaTime, maxSpeed);
		}
		action = TEXT("Accelerating");
	}
	else if (!isAccelerating & isBraking && inGround)
	{
		if (currentSpeed > 0)
		{
			currentSpeed -= acceleration * brakeRate * deltaTime;
		}
		else if (currentSpeed > reverseSpeed)
		{
			currentSpeed = FMath::Min(currentSpeed - acceleration * reverseRate * deltaTime, reverseSpeed);
		}

		action = TEXT("Braking");
	}
	else if (isBraking && isAccelerating && inGround)
	{
		currentSpeed = FMath::Max(currentSpeed - acceleration * brakeRate * deltaTime, 0.f);
		action = TEXT("Braking and Accelerating");
	}
	else
	{
		currentSpeed = FMath::Max(currentSpeed - acceleration * frictionDecelerationRate * deltaTime, 0.f);
		action = TEXT("Nothing");
	}
	if (inGround)
		chassisMesh->SetPhysicsLinearVelocity((GetActorForwardVector() * currentSpeed) + additionalForces*10);

	if (GEngine)
	{
		// GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Yellow,
		//                                  FString::Printf(
		// 	                                 TEXT("Speed: %f"), currentSpeed));
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue, FString::Printf(TEXT("%s"), *action));
	}
}

void AVehiclePawn::GravityForce() const
{
	chassisMesh->AddForce(-GetActorUpVector() * 980, NAME_None, true);
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
	return chassisMesh;
}

URaceComponent* AVehiclePawn::GetRaceComponent() const
{
	return raceComponent;
}
