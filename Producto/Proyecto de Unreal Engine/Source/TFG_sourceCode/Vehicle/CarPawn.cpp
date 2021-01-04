// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn.h"

ACarPawn::ACarPawn()
{
	frontWheels = CreateDefaultSubobject<USceneComponent>(TEXT("Front Wheels"));
	frontWheels->SetupAttachment(RootComponent);
	rearWheels = CreateDefaultSubobject<USceneComponent>(TEXT("Rear Wheels"));
	rearWheels->SetupAttachment(RootComponent);

	rightFrontWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Front Wheel Mesh"));
	rightFrontWheelMesh->SetupAttachment(frontWheels);
	leftFrontWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Front Wheel Mesh"));
	leftFrontWheelMesh->SetupAttachment(frontWheels);
	rightRearWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Rear Wheel Mesh"));
	rightRearWheelMesh->SetupAttachment(rearWheels);
	leftRearWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Rear Wheel Mesh"));
	leftRearWheelMesh->SetupAttachment(rearWheels);
}

void ACarPawn::BeginPlay()
{
	Super::BeginPlay();

	rightFrontWheelMesh->SetSimulatePhysics(true);
	leftFrontWheelMesh->SetSimulatePhysics(true);
	rightRearWheelMesh->SetSimulatePhysics(true);
	leftRearWheelMesh->SetSimulatePhysics(true);
	// wheels[4];
	// wheels[0]=rightFrontWheelMesh;
	// wheels[1]=leftFrontWheelMesh;
	// wheels[2]= rightRearWheelMesh;
	// wheels[3]=leftRearWheelMesh;
}

void ACarPawn::SuspensionForces()
{
	Super::SuspensionForces();

	SingleSuspensionForce(rightFrontWheelMesh->GetComponentLocation(), rightFrontWheelMesh,TEXT("Right Front"));
	SingleSuspensionForce(leftFrontWheelMesh->GetComponentLocation(), leftFrontWheelMesh,TEXT("Left Front"));
	SingleSuspensionForce(rightRearWheelMesh->GetComponentLocation(), rightRearWheelMesh,TEXT("Right Rear"));
	SingleSuspensionForce(leftRearWheelMesh->GetComponentLocation(), leftRearWheelMesh,TEXT("Left Rear"));
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
