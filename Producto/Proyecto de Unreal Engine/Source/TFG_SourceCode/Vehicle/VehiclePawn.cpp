// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include <valarray>


#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/RaceComponent.h"
#include "Components/TyreComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// RootComponent = root;

	carMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh"));
	RootComponent = carMesh;

	objectSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Object Spawn Point"));
	objectSpawnPoint->SetupAttachment(carMesh);

	particleSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Spawn Point"));
	particleSpawnPoint->SetupAttachment(carMesh);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(carMesh);

	normalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	normalCamera->SetupAttachment(springArm);
	normalCamera->Activate();

	reverseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Reverse Camera"));
	reverseCamera->SetupAttachment(carMesh);
	reverseCamera->Deactivate();

	raceComponent = CreateDefaultSubobject<URaceComponent>(TEXT("Race Component"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / reverseRate;
	acceleration = maxSpeed;
	reverseAcceleration = acceleration / reverseRate;
	initialMaxSpeed = maxSpeed;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GravityForce();
	SuspensionForces();

	if (raceComponent->CanRace())
	{
		WaitAfterHit(DeltaTime);

		Movement();
	}
	else
	{
		carMesh->SetLinearDamping(1000000);
		carMesh->SetAngularDamping(1000000);
	}
}

/*
*########
*MOVEMENT
*########
*/

void AVehiclePawn::Movement()
{
	currentSpeed = (GetActorForwardVector() * carMesh->GetPhysicsLinearVelocity()).Size();
	float currentAngular = (carMesh->GetPhysicsAngularVelocityInDegrees() * GetActorUpVector()).Size();
	if (inGround)
	{
		if (!invertControls && isAccelerating && !isBraking || invertControls && !isAccelerating && isBraking)
		{
			deaccelerationTimer = 0;
			PerformAcceleration();
		}
		else if (!invertControls && !isAccelerating && isBraking || invertControls && isAccelerating && !isBraking)
		{
			deaccelerationTimer = 0;
			PerformBraking(currentSpeed);
		}
		else if (isBraking && isAccelerating)
		{
			currentSpeed = lastVelocity < currentSpeed ? -currentSpeed : currentSpeed;
			carMesh->SetLinearDamping(1.f);
			brakeTimer = 0;
			accelerationTimer = 0;
			reverseTimer = 0;


			// carMesh->SetPhysicsLinearVelocity(GetActorForwardVector()*currentSpeed);
			//rotate vehicle
		}
		else
		{
			carMesh->SetLinearDamping(2.f);
			brakeTimer = 0;
			accelerationTimer = 0;
			reverseTimer = 0;
			// deaccelerationTimer += GetWorld()->DeltaTimeSeconds;

			// carMesh->AddForceAtLocation(
			// 	carMesh->GetForwardVector() * acceleration * accelerationRate * 2 / FMath::Exp(deaccelerationTimer),
			// 	GetCenterOfMass());
			// UE_LOG(LogTemp, Warning, TEXT("%f"), (acceleration * accelerationRate / FMath::Exp(deaccelerationTimer)))
		}


		if (isDrifting)
		{
			PerformDrift(currentSpeed);
		}
		else
		{
			PerformSteering(currentSpeed, currentAngular);
		}
	}
	else
	{
		carMesh->SetLinearDamping(.01f);
	}

	for (UTyreComponent* tyre : tyres)
	{
		tyre->RotateTyres(currentSpeed);
	}

	if (lastVelocity < 0)
	{
		normalCamera->Deactivate();
		reverseCamera->Activate();
	}
	else
	{
		reverseCamera->Deactivate();
		normalCamera->Activate();
	}

	//DEBUG
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange,
		                                 FString::Printf(
			                                 TEXT("Angular: %f"), currentAngular));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Speed: %f"), currentSpeed));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
		//                                  FString::Printf(
		// 	                                 TEXT("Expected Checkpoint= %i"), raceComponent->GetExpectedCheckpoint()));
	}
}

/*
*##########
*ACCELERATE
*##########
*/

void AVehiclePawn::Accelerate()
{
	isAccelerating = !isAccelerating;
	if (!isAccelerating)
		accelerationTimer = 0;
}

void AVehiclePawn::PerformAcceleration()
{
	carMesh->SetLinearDamping(1.f);
	if (currentSpeed < maxSpeed)
	{
		carMesh->AddForceAtLocation(
			carMesh->GetForwardVector() * acceleration * FMath::Exp(accelerationTimer) * accelerationRate,
			GetCenterOfMass());
		lastVelocity = currentSpeed;
		accelerationTimer += GetWorld()->DeltaTimeSeconds;
	}
}

/*
*#####
*BRAKE
*#####
*/

void AVehiclePawn::Brake()
{
	isBraking = !isBraking;
	if (!isBraking)
	{
		brakeTimer = 0;
		reverseTimer = 0;
	}
}

void AVehiclePawn::PerformBraking(float& currentVelocity)
{
	carMesh->SetLinearDamping(1.f);
	currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
	GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
	                                 FString::Printf(
		                                 TEXT("Object= %f"),
		                                 currentVelocity));
	if (currentVelocity < maxSpeed / 20 && currentVelocity > reverseSpeed)
	{
		carMesh->AddForceAtLocation(
			-carMesh->GetForwardVector() * acceleration * FMath::Exp(reverseTimer) * accelerationRate,
			GetCenterOfMass());
		lastVelocity = currentVelocity;
		brakeTimer = 0;
		reverseTimer += GetWorld()->DeltaTimeSeconds;
	}
	else
	{
		carMesh->AddForceAtLocation(-carMesh->GetForwardVector() * acceleration * FMath::Exp(brakeTimer) * brakeRate,
		                            GetCenterOfMass());
		lastVelocity = currentVelocity;
		reverseTimer = 0;
		brakeTimer += GetWorld()->DeltaTimeSeconds;
	}
}

/*
*#####
*STEER
*#####
*/

void AVehiclePawn::Steer(float value)
{
	steerValue = invertControls ? -value : value;
}

void AVehiclePawn::PerformSteering(float currentVelocity, float currentAngular)
{
	driftValue = 0;
	carMesh->SetAngularDamping(10.f);
	float x = currentVelocity / maxSpeed;
	float steeringAngle = maxSteerAngle;
	if (x < 0) steeringAngle *= FMath::Sin(-4 * x);
	else if (x < 0.25) steeringAngle *= FMath::Sin(1885 * x / 300);
	else steeringAngle *= FMath::Sin(x + 1.5);

	carMesh->SetPhysicsMaxAngularVelocityInDegrees(steeringAngle);

	// if (FMath::Abs(steerValue) == 0)
	// {
	// 	if (currentAngular <= maxSteerAngle * frictionDecelerationRate)
	// 	{
	// 		lastTurnValue = 0;
	// 	}
	// }
	// else
	// {
	carMesh->AddTorqueInDegrees(
		carMesh->GetUpVector() * steeringRate * 2 * steerValue * steeringAngle / maxDriftAngle,
		NAME_None, true);
	// 	lastTurnValue = FMath::Sign(steerValue);
	// }

	for (UTyreComponent* tyre : tyres)
	{
		if (Traction4x4 || tyre->GetName().Contains("Front"))
			tyre->Steer(steerValue);
	}
}

/*
*#####
*DRIFT
*#####
*/

void AVehiclePawn::Drift()
{
	if (turnTimer < turnToDriftSeconds)
	{
		isDrifting = !isDrifting;
		if (isDrifting)
		{
			driftSign = FMath::Sign(steerValue);
			springArm->AddLocalRotation(FRotator(0, -driftSign * cameraRotation, 0));
		}
		else
		{
			springArm->AddLocalRotation(FRotator(0, driftSign * cameraRotation, 0));
		}
	}
	for (UTyreComponent* tyre : tyres)
	{
		if (Traction4x4 || tyre->GetName().Contains("Front"))
			tyre->Drift(steerValue);
	}
}

void AVehiclePawn::PerformDrift(float currentVelocity)
{
	carMesh->SetAngularDamping(5.f);
	if (driftSign == 0 || FMath::Abs(currentVelocity) < maxSpeed / reverseRate || turnTimer >= turnToDriftSeconds)
	{
		isDrifting = false;
		turnTimer = 0;
		driftTimer = 0;
		driftInverseTimer = 0;
		springArm->AddLocalRotation(FRotator(0, driftSign * cameraRotation, 0));
		return;
	}

	carMesh->SetPhysicsMaxAngularVelocityInDegrees(CalculateMaxDriftValue());

	carMesh->AddTorqueInDegrees(carMesh->GetUpVector() * steeringRate * driftSign, NAME_None, true);

	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Front"))
			tyre->Drift(steerValue);
	}
}

float AVehiclePawn::CalculateMaxDriftValue()
{
	if (driftValue == 0)
		driftValue = maxSteerAngle;

	if (steerValue == 0)
	{
		driftTimer = 0;
		driftInverseTimer = 0;
	}

	if (FMath::Sign(steerValue) == driftSign)
	{
		driftInverseTimer = 0;
		driftTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
		driftRateIncrease = 20;
		driftValue += (FMath::Exp(driftTimer / driftRateIncrease) - 1);
		UE_LOG(LogTemp, Warning, TEXT("%f"), (FMath::Exp(driftTimer/10) - 1))
	}
	else
	{
		driftTimer = 0;
		driftInverseTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
		driftRateDecrease = 40;
		driftValue -= (FMath::Exp(driftInverseTimer / driftRateDecrease) - 1);
		UE_LOG(LogTemp, Error, TEXT("%f"), (FMath::Exp(driftInverseTimer/20) - 1))
	}

	return driftValue >= maxDriftAngle
		       ? maxDriftAngle
		       : driftValue <= maxSteerAngle
					? maxSteerAngle
					: driftSign * driftValue;
}

/*
*#######
*PHYSICS
*#######
*/

void AVehiclePawn::GravityForce() const
{
	carMesh->AddForce(GetActorUpVector() * GetWorld()->GetGravityZ(), NAME_None, true);
}

void AVehiclePawn::SuspensionForces()
{
	inGround = false;
	for (UTyreComponent* tyre : tyres)
	{
		bool temp = tyre->SuspensionForce(suspensionDistance, suspensionRate, dampingRate);
		if (temp)
		{
			inGround = true;
		}
	}
}

/*
*####### #######
*OBJECTS RELATED
*####### #######
*/

void AVehiclePawn::UseObject()
{
	if (currentObject && !hasBeenHit)
	{
		currentObject->UseObject();
	}
}

void AVehiclePawn::RemoveObject()
{
	currentObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	currentObject->SetOwner(nullptr);
	currentObject = nullptr;
}

void AVehiclePawn::Damage()
{
	hasBeenHit = true;
	maxSpeed = 0;
}

void AVehiclePawn::WaitAfterHit(float DeltaTime)
{
	if (hasBeenHit)
	{
		if (hitTimer >= hitWaiting)
		{
			hasBeenHit = false;
			maxSpeed = initialMaxSpeed;
			hitTimer = 0;
		}
		hitTimer += DeltaTime;
	}
}

void AVehiclePawn::InstantiateParticle(const TSubclassOf<AActor>& particle)
{
	currentParticle = GetWorld()->SpawnActor<AActor>(
		particle,
		particleSpawnPoint->GetComponentLocation(),
		particleSpawnPoint->GetComponentRotation()
	);
	currentParticle->AttachToComponent(particleSpawnPoint, FAttachmentTransformRules::KeepWorldTransform);
}

void AVehiclePawn::InvertControls()
{
	invertControls = true;
}

void AVehiclePawn::NormalControls()
{
	currentParticle->Destroy();
	invertControls = false;
}

/*
 *####### ### #######
 *GETTERS AND SETTERS
 *####### ### #######
 */

FVector AVehiclePawn::GetForward() const
{
	FHitResult hit;
	FVector position = GetActorLocation();
	FVector end = position - (GetActorUpVector() * 300);
	FVector forward = GetActorForwardVector();

	GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);

	if (hit.bBlockingHit)
	{
		forward = GetActorForwardVector() - hit.Normal * GetActorForwardVector();
	}

	return forward;
}

UStaticMeshComponent* AVehiclePawn::GetMesh() const
{
	return carMesh;
}

URaceComponent* AVehiclePawn::GetRaceComponent() const
{
	return raceComponent;
}

AObjectBase* AVehiclePawn::GetCurrentObject() const
{
	return currentObject;
}

void AVehiclePawn::SetCurrentObject(AObjectBase* CurrentObject)
{
	if (this->currentObject)
	{
		this->currentObject->Destroy();
	}
	if (CurrentObject)
	{
		this->currentObject = CurrentObject;
		this->currentObject->SetVehicle(this);
		this->currentObject->SetOwner(this);
		this->currentObject->SetActorLocation(objectSpawnPoint->GetComponentLocation());
		this->currentObject->AttachToComponent(objectSpawnPoint, FAttachmentTransformRules::KeepWorldTransform);
	}
}

float AVehiclePawn::GetMaxSpeed() const
{
	return maxSpeed;
}

void AVehiclePawn::SetMaxSpeed(float speed)
{
	maxSpeed = speed;
	acceleration = (maxSpeed / accelerationRate) * 200;
}

float AVehiclePawn::GetInitialMaxSpeed() const
{
	return initialMaxSpeed;
}

FVector AVehiclePawn::GetCenterOfMass() const
{
	return carMesh->GetCenterOfMass() - GetActorUpVector() * 10 +
		GetActorForwardVector() * 10;
}

bool AVehiclePawn::GetDrifting() const
{
	return isDrifting;
}

bool AVehiclePawn::GetBraking()
{
	return isBraking;
}

float AVehiclePawn::GetCurrentSpeed()
{
	return currentSpeed;
}

float AVehiclePawn::GetDriftSign()
{
	return driftSign;
}

bool AVehiclePawn::GetHasBeenHit() const
{
	return hasBeenHit;
}

bool AVehiclePawn::GetIsAccelerating() const
{
	return isAccelerating;
}
