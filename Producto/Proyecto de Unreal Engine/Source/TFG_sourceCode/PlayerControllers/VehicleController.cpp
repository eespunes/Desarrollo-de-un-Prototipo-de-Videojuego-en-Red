// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"

#include "Blueprint/UserWidget.h"

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
	
	UUserWidget* HUD = CreateWidget(this, hudClass);
	HUD->AddToViewport();
}

void AVehicleController::SetupInputComponent()
{
	Super::SetupInputComponent();

	Actions();

	Axis();
}

void AVehicleController::Actions()
{
	InputComponent->BindAction("Accelerate", IE_Pressed, this, &AVehicleController::Accelerate);
	InputComponent->BindAction("Accelerate", IE_Released, this, &AVehicleController::Accelerate);
	InputComponent->BindAction("Brake", IE_Pressed, this, &AVehicleController::Brake);
	InputComponent->BindAction("Brake", IE_Released, this, &AVehicleController::Brake);
}

void AVehicleController::Axis()
{
	InputComponent->BindAxis("Turn", this, &AVehicleController::Turn);
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

void AVehicleController::Brake()
{
	if (!vehicleCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetOwner()->GetName());
		return;
	}

	vehicleCharacter->Brake();
}

void AVehicleController::Turn(float value)
{
	if (!vehicleCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetOwner()->GetName());
		return;
	}

	vehicleCharacter->Turn(value);
}
