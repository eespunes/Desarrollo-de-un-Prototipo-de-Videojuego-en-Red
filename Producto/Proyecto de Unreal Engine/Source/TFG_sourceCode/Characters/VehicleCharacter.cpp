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
}

// Called every frame
void AVehicleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAccelerating)
	{
		currentSpeed = FMath::Min(currentSpeed + acceleration * DeltaTime, maxSpeed);
	}
	else
	{
		currentSpeed = FMath::Max(currentSpeed - acceleration * DeltaTime, 0.f);
	}
	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), currentSpeed);
	AddMovementInput(GetActorForwardVector() * (currentSpeed / maxSpeed));
}

void AVehicleCharacter::Accelerate()
{
	isAccelerating = !isAccelerating;
}
