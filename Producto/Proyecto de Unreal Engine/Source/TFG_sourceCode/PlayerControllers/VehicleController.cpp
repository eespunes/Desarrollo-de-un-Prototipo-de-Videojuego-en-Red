// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"

AVehicleController::AVehicleController()
{
	// springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	// RootComponent->AttachTo(springArm);
	//
	// camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// camera->SetupAttachment(springArm);
}

void AVehicleController::BeginPlay()
{
	Super::BeginPlay();
	vehicleCharacter = Cast<AVehicleCharacter>(GetPawn());
}

void AVehicleController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Accelerate", IE_Pressed, this, &AVehicleController::Accelerate);
	InputComponent->BindAction("Accelerate", IE_Released, this, &AVehicleController::Accelerate);
}

void AVehicleController::Accelerate()
{
	if (!vehicleCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetOwner()->GetName());
		return;
	}

	vehicleCharacter->Accelerate();
}
