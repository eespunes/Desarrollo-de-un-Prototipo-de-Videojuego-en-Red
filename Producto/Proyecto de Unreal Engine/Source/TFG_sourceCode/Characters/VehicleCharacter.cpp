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
	frictionDeceleration = acceleration / frictionDecelerationRate;
}

// Called every frame
void AVehicleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateSpeed(DeltaTime);

	AddMovementInput(GetActorForwardVector() * (currentSpeed / maxSpeed));
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
	if (currentSpeed < 1 && currentSpeed > -1)
		return 0.f;
	float interval = maxTurnSpeed - minTurnSpeed;
	interval *= currentSpeed / maxSpeed;
	return value * (maxTurnSpeed - interval);
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
		if (currentSpeed < 0.f)
			currentSpeed += frictionDeceleration * DeltaTime;
		else
			currentSpeed = FMath::Max(currentSpeed - frictionDeceleration * DeltaTime, 0.f);
	}
}

float AVehicleCharacter::GetSpeed() const
{
	return currentSpeed / 100;
}
