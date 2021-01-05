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
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	lastUpVector = GetActorUpVector();

	// reverseSpeed = -maxSpeed / reverseRate;
	// acceleration = maxSpeed / accelerationRate;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CalculateSpeed(DeltaTime);
	GravityForce();
	SuspensionForces();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow,
		                                 FString::Printf(TEXT("Velocity: %f"), GetVelocity().Size()));
	}
}

void AVehiclePawn::Accelerate()
{
	// isAccelerating = !isAccelerating;
}

void AVehiclePawn::Brake()
{
	// isBraking = !isBraking;
}

void AVehiclePawn::Turn(float value)
{
	// AddControllerYawInput(CalculateRotation(value) * GetWorld()->DeltaTimeSeconds);
}

void AVehiclePawn::Drift()
{
}

float AVehiclePawn::CalculateRotation() const
{
	// float percentage = FMath::Abs(currentSpeed) / maxSpeed;
	// if (percentage > frictionDecelerationRate)
	// 	percentage = 1;
	// else
	// 	percentage /= frictionDecelerationRate;
	// return value * (maxTurnSpeed * percentage);
	return 0;
}

void AVehiclePawn::CalculateSpeed()
{
	// if (isAccelerating && !isBraking)
	// {
	// 	if (currentSpeed < 0.f)
	// 		currentSpeed += acceleration * brakeRate * DeltaTime;
	// 	currentSpeed = FMath::Min(currentSpeed + (acceleration) * DeltaTime, maxSpeed);
	// }
	// else if (isBraking && !isAccelerating)
	// {
	// 	currentSpeed -= acceleration * brakeRate * DeltaTime;
	// 	if (currentSpeed <= 0.f)
	// 		currentSpeed = FMath::Max(currentSpeed - acceleration * DeltaTime, reverseSpeed);
	// }
	// else if (isBraking && isAccelerating)
	// {
	// 	if (currentSpeed < 0.f)
	// 		currentSpeed += acceleration * brakeRate * DeltaTime;
	// 	else
	// 		currentSpeed = FMath::Max(currentSpeed - acceleration * brakeRate * DeltaTime, 0.f);
	// }
	// else
	// {
	// 	FrictionBraking(DeltaTime);
	// }
	//
	// if (GetCharacterMovement())
	// 	GetCharacterMovement()->MaxWalkSpeed = FMath::Abs(currentSpeed);
}


void AVehiclePawn::FrictionBraking()
{
	// float percentage = FMath::Abs(currentSpeed) / maxSpeed;
	// if (percentage > frictionDecelerationRate)
	// 	percentage = 1;
	//
	// if (currentSpeed < 0.f)
	// 	currentSpeed += acceleration * percentage * DeltaTime;
	// else
	// 	currentSpeed = FMath::Max(currentSpeed - acceleration * percentage * DeltaTime, 0.f);
}

void AVehiclePawn::GravityForce() const
{
	chassisMesh->AddForce(-lastUpVector * 980, NAME_None, true);
}

void AVehiclePawn::SuspensionForces()
{
}

float AVehiclePawn::GetSpeed() const
{
	return currentSpeed;
}
