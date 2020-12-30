// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleCharacter.h"

// Sets default values
AVehicleCharacter::AVehicleCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void AVehicleCharacter::BeginPlay()
{
	Super::BeginPlay();
	reverseSpeed = -maxSpeed / reverseRate;
	acceleration = maxSpeed / accelerationRate;

	if (GetCharacterMovement())
		GetCharacterMovement()->MaxAcceleration = FMath::Abs(acceleration);
}

// Called every frame
void AVehicleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateSpeed(DeltaTime);

	AddMovementInput(GetActorForwardVector(), currentSpeed * DeltaTime);
}

void AVehicleCharacter::Accelerate()
{
	isAccelerating = !isAccelerating;
}

void AVehicleCharacter::Brake()
{
	isBraking = !isBraking;
}

void AVehicleCharacter::Turn(float value)
{
	AddControllerYawInput(CalculateRotation(value) * GetWorld()->DeltaTimeSeconds);
}

float AVehicleCharacter::CalculateRotation(float value) const
{
	float percentage = FMath::Abs(currentSpeed) / maxSpeed;
	if (percentage > frictionDecelerationRate)
		percentage = 1;
	else
		percentage /= frictionDecelerationRate;
	return value * (maxTurnSpeed * percentage);
}

void AVehicleCharacter::CalculateSpeed(float DeltaTime)
{
	if (isAccelerating && !isBraking)
	{
		if (currentSpeed < 0.f)
			currentSpeed += acceleration * brakeRate * DeltaTime;
		currentSpeed = FMath::Min(currentSpeed + (acceleration) * DeltaTime, maxSpeed);
	}
	else if (isBraking && !isAccelerating)
	{
		currentSpeed -= acceleration * brakeRate * DeltaTime;
		if (currentSpeed <= 0.f)
			currentSpeed = FMath::Max(currentSpeed - acceleration * DeltaTime, reverseSpeed);
	}
	else if (isBraking && isAccelerating)
	{
		if (currentSpeed < 0.f)
			currentSpeed += acceleration * brakeRate * DeltaTime;
		else
			currentSpeed = FMath::Max(currentSpeed - acceleration * brakeRate * DeltaTime, 0.f);
	}
	else
	{
		FrictionBraking(DeltaTime);
	}

	if (GetCharacterMovement())
		GetCharacterMovement()->MaxWalkSpeed = FMath::Abs(currentSpeed);
}


void AVehicleCharacter::FrictionBraking(float DeltaTime)
{
	float percentage = FMath::Abs(currentSpeed) / maxSpeed;
	if (percentage > frictionDecelerationRate)
		percentage = 1;

	if (currentSpeed < 0.f)
		currentSpeed += acceleration * percentage * DeltaTime;
	else
		currentSpeed = FMath::Max(currentSpeed - acceleration * percentage * DeltaTime, 0.f);
}

float AVehicleCharacter::GetSpeed() const
{
	return currentSpeed / 100;
}
