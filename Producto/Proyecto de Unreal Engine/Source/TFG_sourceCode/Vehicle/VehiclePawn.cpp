// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include "DrawDebugHelpers.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chassisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	RootComponent = chassisMesh;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(chassisMesh);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / 3;
	acceleration = (maxSpeed / accelerationRate) * 10;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateSpeed();
	GravityForce();
	SuspensionForces();
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
		if (angular < maxDriftAngle)
			chassisMesh->SetAngularDamping(.01f);
		chassisMesh->AddTorqueInDegrees(GetActorUpVector() * value * minTurnSpeed, NAME_None, true);
	}
	else
	{
		if (value == 0)
		{
			chassisMesh->SetAngularDamping(100.f);
		}
			// else if (value == 0 && angular <= 10)
			// {
			// 	chassisMesh->SetPhysicsAngularVelocity(FVector(0, 0, 0));
			// }
		else if (angular < maxTurnAngle)
		{
			chassisMesh->SetAngularDamping(.01f);
			chassisMesh->AddTorqueInDegrees(GetActorUpVector() * value * minTurnSpeed, NAME_None, true);
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
			                                 TEXT("Angular: %f"), angular));
	}
}

void AVehiclePawn::Drift()
{
	isDrifting = !isDrifting;
}

float AVehiclePawn::CalculateRotation(float value) const
{
	float percentage = FMath::Abs(lastVelocity) / maxSpeed;
	if (percentage > frictionDecelerationRate)
		percentage = 1;
	else
		percentage /= frictionDecelerationRate;
	return value * (maxTurnAngle * percentage);
}

void AVehiclePawn::CalculateSpeed()
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float velocitySize = chassisMesh->GetPhysicsLinearVelocity().Size();
	float currentVelocity = FVector(GetActorForwardVector().X * velocitySize,
	                                GetActorForwardVector().Y * velocitySize, 0).Size();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Velocity: %f"), currentVelocity));
	}
	if (isAccelerating & !isBraking && inGround)
	{
		chassisMesh->SetLinearDamping(10.f);
		if (currentVelocity < maxSpeed)
		{
			chassisMesh->AddForce(GetActorForwardVector() * acceleration, NAME_None, true);
		}
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue, FString::Printf(TEXT("Accelerating")));
	}
	else if (!isAccelerating & isBraking && inGround)
	{
		chassisMesh->SetLinearDamping(1.f);

		currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
		if (currentVelocity < (maxSpeed / 10))
			if (currentVelocity > reverseSpeed)
				chassisMesh->AddForce(GetActorForwardVector() * -acceleration * brakeRate, NAME_None, true);

		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue, FString::Printf(TEXT("Braking")));
	}
	else if (isBraking && isAccelerating && inGround)
	{
		chassisMesh->SetLinearDamping(1.f);
	}
	else
	{
		chassisMesh->SetLinearDamping(.5f);
	}
	// chassisMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * currentVelocity);

	lastVelocity = currentVelocity;
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
