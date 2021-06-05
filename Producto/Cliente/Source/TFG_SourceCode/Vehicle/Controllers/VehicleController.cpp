// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"

#include "Blueprint/UserWidget.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

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

	InputComponent->BindAction("Use Object", IE_Pressed, this, &AVehicleController::UseObject);

	InputComponent->BindAction("Pause", IE_Pressed, this, &AVehicleController::Pause);
}

void AVehicleController::Axis()
{
	InputComponent->BindAxis("Steer", this, &AVehicleController::Steer);
}

void AVehicleController::Accelerate()
{
	if (!vehiclePawn)return;

	vehiclePawn->Accelerate();
}

void AVehicleController::Brake()
{
	if (!vehiclePawn)return;

	vehiclePawn->Brake();
}

void AVehicleController::Steer(float value)
{
	if (!vehiclePawn) return;

	vehiclePawn->Steer(value);
}

void AVehicleController::Drift()
{
	if (!vehiclePawn) return;

	vehiclePawn->Drift();
}

void AVehicleController::UseObject()
{
	if (!vehiclePawn)return;

	vehiclePawn->UseObject();
}

void AVehicleController::Pause()
{
	if (!IsPaused())
	{
		UUserWidget* pauseHUD = CreateWidget(this, pauseClass);
		pauseHUD->AddToViewport();
		SetPause(true);
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
	}
}
