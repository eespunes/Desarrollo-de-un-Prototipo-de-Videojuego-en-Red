// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn.h"

#include "WheelComponent.h"

ACarPawn::ACarPawn()
{
	rightFrontWheel = CreateDefaultSubobject<UWheelComponent>(TEXT("Right Front Wheel"));
	rightFrontWheel->SetupAttachment(RootComponent);
	rightFrontWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Front Wheel Mesh"));
	rightFrontWheelMesh->SetupAttachment(rightFrontWheel);

	leftFrontWheel = CreateDefaultSubobject<UWheelComponent>(TEXT("Left Front Wheel"));
	leftFrontWheel->SetupAttachment(RootComponent);
	leftFrontWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Front Wheel Mesh"));
	leftFrontWheelMesh->SetupAttachment(leftFrontWheel);

	rightRearWheel = CreateDefaultSubobject<UWheelComponent>(TEXT("Right Rear Wheel"));
	rightRearWheel->SetupAttachment(RootComponent);
	rightRearWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Rear Wheel Mesh"));
	rightRearWheelMesh->SetupAttachment(rightRearWheel);

	leftRearWheel = CreateDefaultSubobject<UWheelComponent>(TEXT("Left Rear Wheel"));
	leftRearWheel->SetupAttachment(RootComponent);
	leftRearWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Rear Wheel Mesh"));
	leftRearWheelMesh->SetupAttachment(leftRearWheel);
}

void ACarPawn::BeginPlay()
{
	Super::BeginPlay();

	// wheels[4];
	// wheels[0]=rightFrontWheelMesh;
	// wheels[1]=leftFrontWheelMesh;
	// wheels[2]= rightRearWheelMesh;
	// wheels[3]=leftRearWheelMesh;
}

void ACarPawn::SuspensionForces()
{
	Super::SuspensionForces();

	bool rightFront = rightFrontWheel->SuspensionForce(suspensionDistance, suspensionRate, dampingRate);
	bool leftFront = leftFrontWheel->SuspensionForce(suspensionDistance, suspensionRate, dampingRate);
	bool rightRear = rightRearWheel->SuspensionForce(suspensionDistance, suspensionRate, dampingRate);
	bool leftRear = leftRearWheel->SuspensionForce(suspensionDistance, suspensionRate, dampingRate);

	inGround = false;
	if (rightFront && leftFront && rightRear && leftRear) inGround = true;
}

void ACarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACarPawn::Accelerate()
{
	Super::Accelerate();
}

void ACarPawn::Brake()
{
	Super::Brake();
}

void ACarPawn::Turn(float value)
{
	Super::Turn(value);
}

void ACarPawn::Drift()
{
	Super::Drift();
}
