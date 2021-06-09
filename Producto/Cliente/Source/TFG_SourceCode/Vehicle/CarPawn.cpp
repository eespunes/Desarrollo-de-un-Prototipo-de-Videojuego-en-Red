// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn.h"

#include "Components/TyreComponent.h"

ACarPawn::ACarPawn()
{
	rightFrontWheel = CreateDefaultSubobject<UTyreComponent>(TEXT("Right Front Tyre"));
	rightFrontWheel->SetupAttachment(Super::carMesh);
	tyres.Add(rightFrontWheel);
	rightFrontWheelRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Right Front Tyre Root"));
	rightFrontWheelRoot->SetupAttachment(rightFrontWheel);
	rightFrontWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Front Tyre Mesh"));
	rightFrontWheelMesh->SetupAttachment(rightFrontWheelRoot);
	
	rightFrontWheel->SetMesh(rightFrontWheelMesh);
	rightFrontWheel->SetRootPoint(rightFrontWheelRoot);

	leftFrontWheel = CreateDefaultSubobject<UTyreComponent>(TEXT("Left Front Tyre"));
	leftFrontWheel->SetupAttachment(Super::carMesh);
	tyres.Add(leftFrontWheel);
	leftFrontWheelRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Left Front Tyre Root"));
	leftFrontWheelRoot->SetupAttachment(leftFrontWheel);
	leftFrontWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Front Tyre Mesh"));
	leftFrontWheelMesh->SetupAttachment(leftFrontWheelRoot);

	leftFrontWheel->SetMesh(leftFrontWheelMesh);
	leftFrontWheel->SetRootPoint(leftFrontWheelRoot);

	rightRearWheel = CreateDefaultSubobject<UTyreComponent>(TEXT("Right Rear Tyre"));
	rightRearWheel->SetupAttachment(Super::carMesh);
	tyres.Add(rightRearWheel);
	rightRearWheelRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Right Rear Tyre Root"));
	rightRearWheelRoot->SetupAttachment(rightRearWheel);
	rightRearWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Rear Tyre Mesh"));
	rightRearWheelMesh->SetupAttachment(rightRearWheelRoot);

	rightRearWheel->SetMesh(rightRearWheelMesh);
	rightRearWheel->SetRootPoint(rightRearWheelRoot);

	leftRearWheel = CreateDefaultSubobject<UTyreComponent>(TEXT("Left Rear Tyre"));
	leftRearWheel->SetupAttachment(Super::carMesh);
	tyres.Add(leftRearWheel);
	leftRearWheelRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Left Rear Tyre Root"));
	leftRearWheelRoot->SetupAttachment(leftRearWheel);
	leftRearWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Rear Tyre Mesh"));
	leftRearWheelMesh->SetupAttachment(leftRearWheelRoot);

	leftRearWheel->SetMesh(leftRearWheelMesh);
	leftRearWheel->SetRootPoint(leftRearWheelRoot);
}
