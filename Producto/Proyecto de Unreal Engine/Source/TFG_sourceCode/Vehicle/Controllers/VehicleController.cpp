// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"

#include "Blueprint/UserWidget.h"

AVehicleController::AVehicleController()
{
}

void AVehicleController::BeginPlay()
{
	Super::BeginPlay();
	vehiclePawn = Cast<AVehiclePawn>(GetPawn());
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

	InputComponent->BindAction("Drift", IE_Pressed, this, &AVehicleController::Drift);
	InputComponent->BindAction("Drift", IE_Released, this, &AVehicleController::Drift);

	InputComponent->BindAction("Pause", IE_Pressed, this, &AVehicleController::Pause);
}

void AVehicleController::Axis()
{
	InputComponent->BindAxis("Turn", this, &AVehicleController::Turn);
}

void AVehicleController::Accelerate()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Accelerate();
}

void AVehicleController::Brake()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Brake();
}

void AVehicleController::Turn(float value)
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Turn(value);
}

void AVehicleController::Drift()
{
	if (!vehiclePawn)
	{
		UE_LOG(LogTemp, Error, TEXT("There's no AVehiclePawn instance in %s"), *GetName());
		return;
	}

	vehiclePawn->Drift();
}

void AVehicleController::Pause()
{
	if(!IsPaused())
	{
		UUserWidget* pauseHUD = CreateWidget(this, pauseClass);
		pauseHUD->AddToViewport();
		SetPause(true);
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
	}
}
