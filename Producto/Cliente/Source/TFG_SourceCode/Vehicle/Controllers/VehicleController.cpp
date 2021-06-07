// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AVehicleController::BeginPlay()
{
	Super::BeginPlay();
	vehiclePawn = Cast<AVehiclePawn>(GetPawn());
	UUserWidget* HUD = CreateWidget(this, hudClass);
	HUD->AddToViewport();

	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
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
	if (!vehiclePawn || (vehiclePawn && vehiclePawn->GetRaceComponent()->HasFinished()))return;

	vehiclePawn->Accelerate();
}

void AVehicleController::Brake()
{
	if (!vehiclePawn || (vehiclePawn && vehiclePawn->GetRaceComponent()->HasFinished()))return;

	vehiclePawn->Brake();
}

void AVehicleController::Steer(float value)
{
	if (!vehiclePawn || (vehiclePawn && vehiclePawn->GetRaceComponent()->HasFinished())) return;

	vehiclePawn->Steer(value);
}

void AVehicleController::Drift()
{
	if (!vehiclePawn || (vehiclePawn && vehiclePawn->GetRaceComponent()->HasFinished())) return;

	vehiclePawn->Drift();
}

void AVehicleController::UseObject()
{
	if (!vehiclePawn || (vehiclePawn && vehiclePawn->GetRaceComponent()->HasFinished()))return;

	vehiclePawn->UseObject();
}

void AVehicleController::Pause()
{
	if (!gameInstance->GetPauseMenuActivated() && (vehiclePawn && !vehiclePawn->GetRaceComponent()->HasFinished()))
	{
		UUserWidget* pauseHUD = CreateWidget(this, pauseClass);
		pauseHUD->AddToViewport();
		if (!gameInstance->IsMultiplayer())
			SetPause(true);
		gameInstance->SetPauseMenuActivated(true);
		pauseMenuActivated = true;
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
	}
}
