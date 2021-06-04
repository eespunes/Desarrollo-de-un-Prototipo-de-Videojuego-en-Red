// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include <valarray>


#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/NetworkComponent.h"
#include "Components/RaceComponent.h"
#include "Components/TyreComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "Misc/TextFilterExpressionEvaluator.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
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

	playerNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player Name"));
	playerNameText->SetupAttachment(carMesh);

	raceComponent = CreateDefaultSubobject<URaceComponent>(TEXT("Race Component"));
	networkComponent = CreateDefaultSubobject<UNetworkComponent>(TEXT("Network Component"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInstance)
	{
		maxSpeed = gameInstance->GetDifficulty();
	}

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / reverseRate;
	initialMaxSpeed = maxSpeed;
	accelerationTimer = 1;
	brakeTimer = 1;
	decelerationTimer = 1;
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
	carMesh->SetLinearDamping(1);
	carMesh->SetAngularDamping(1);


	GetMesh()->AddLocalRotation(FRotator(-inPitch, 0, 0));
	inPitch = 0;

	if (inGround)
	{
		if (!invertControls && isAccelerating && !isBraking || invertControls && !isAccelerating && isBraking)
		{
			decelerationTimer = 1;

			currentSpeed = FMath::Min<float>(
				currentSpeed + maxSpeed * (FMath::Exp(accelerationTimer / accelerationRate) - 1) * GetWorld()->
				DeltaTimeSeconds, maxSpeed * terrainFriction);

			accelerationTimer += GetWorld()->DeltaTimeSeconds;

			// inPitchValue = FMath::Min<float>(inPitchValue + maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed),
			//                                  maxPitchRotation);
			// inPitch = inPitchValue;
			// GetMesh()->AddLocalRotation(FRotator(inPitch, 0, 0));
		}
		else if (!invertControls && !isAccelerating && isBraking || invertControls && isAccelerating && !isBraking)
		{
			decelerationTimer = 1;

			currentSpeed = FMath::Max<float>(
				currentSpeed - maxSpeed * (FMath::Exp(brakeTimer / brakeRate) - 1) * GetWorld()->DeltaTimeSeconds,
				reverseSpeed);
			brakeTimer += GetWorld()->DeltaTimeSeconds;

			// inPitchValue = FMath::Max<float>(inPitchValue - maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed),
			//                                  -maxPitchRotation);
			// inPitch = inPitchValue;
			// GetMesh()->AddLocalRotation(FRotator(inPitch, 0, 0));
		}
		else
		{
			float exponentialValue = (FMath::Exp(decelerationTimer / decelerationRate) - 1);
			if (currentSpeed > 0)
			{
				currentSpeed = FMath::Max<float>(
					currentSpeed - maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

				// inPitchValue = FMath::Max<float>(
				// 	inPitchValue - maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed), 0);
				// inPitch = inPitchValue;
				// GetMesh()->AddLocalRotation(FRotator(inPitch, 0, 0));
			}
			else if (currentSpeed < 0)
			{
				currentSpeed = FMath::Min<float>(
					currentSpeed + maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

				// inPitchValue = FMath::Min<float>(
				// 	inPitchValue + maxPitchRotation * (FMath::Abs(currentSpeed) / maxSpeed), 0);
				// inPitch = inPitchValue;
				// GetMesh()->AddLocalRotation(FRotator(inPitch, 0, 0));
			}
			decelerationTimer += GetWorld()->DeltaTimeSeconds;
		}


		if (isDrifting)
		{
			if (driftValue >= finalDriftingRate / 2)
			{
				if (!canDriftBoost)
				{
					InstantiateDriftBoostParticles();
					canDriftBoost = true;
				}
			}

			if (FMath::Sign(steerValue) == driftSign)
			{
				driftDecreaseTimer = 0;
				driftIncreaseTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
				driftValue = FMath::Min(driftValue + (FMath::Exp(driftIncreaseTimer / driftRateIncrease) - 1),
				                        finalDriftingRate);
			}
			else
			{
				driftIncreaseTimer = 0;
				driftDecreaseTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
				driftValue = FMath::Max(driftValue - (FMath::Exp(driftDecreaseTimer / driftRateDecrease) - 1),
				                        initialDriftingRate);
			}

			GetMesh()->AddLocalRotation(FRotator(0, driftSign * driftValue * GetWorld()->DeltaTimeSeconds, 0));
			for (UTyreComponent* tyre : tyres)
			{
				if (traction4x4 || tyre->GetName().Contains("Front"))
					tyre->Drift(steerValue);
			}
		}
		else
		{
			GetMesh()->AddLocalRotation(
				FRotator(0, steeringRate * steerValue * (currentSpeed / maxSpeed) * GetWorld()->DeltaTimeSeconds, 0));
			for (UTyreComponent* tyre : tyres)
			{
				if (traction4x4 || tyre->GetName().Contains("Front"))
					tyre->Steer(steerValue);
			}
		}

		if (performDriftBoost)
		{
			if (driftBoostTimer > driftBoostTime)
			{
				performDriftBoost = false;
				SetMaxSpeed(initialMaxSpeed);
				driftBoostTimer = 0;
			}
			else
			{
				driftBoostTimer += GetWorld()->DeltaTimeSeconds;
			}
		}

		carMesh->SetPhysicsLinearVelocity(GetForward() * currentSpeed);
	}
	else
	{
		float exponentialValue = (FMath::Exp(decelerationTimer / decelerationRate) - 1);
		if (currentSpeed > 0)
			currentSpeed = FMath::Max<float>(
				currentSpeed - maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);
		else if (currentSpeed < 0)
			currentSpeed = FMath::Min<float>(
				currentSpeed + maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

		carMesh->AddForce(GetForward() * currentSpeed / flyRate, NAME_None, true);
	}

	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Left"))
			tyre->RotateTyres(-currentSpeed / 2);
		else
			tyre->RotateTyres(currentSpeed / 2);
	}

	if (currentSpeed < 0)
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
			                                 TEXT("Angular: %f"), inPitchValue));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Speed: %f"), currentSpeed));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
		//                                  FString::Printf(
		// 	                                 TEXT("%s Position= (%f,%f,%f)"), *networkComponent->username,
		// 	                                 GetActorLocation().X, GetActorLocation().Y,
		// 	                                 GetActorLocation().Z));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("%s Rotation= (%f,%f,%f)"), *networkComponent->username,
			                                 GetMesh()->GetRelativeRotation().Pitch,
			                                 GetMesh()->GetRelativeRotation().Yaw,
			                                 GetMesh()->GetRelativeRotation().Roll));
	}
}

/*
*##########
*ACCELERATE
*##########
*/

void AVehiclePawn::Accelerate()
{
	networkComponent->SetDataIsAccelerating();
	isAccelerating = networkComponent->GetDataIsAccelerating();
	if (!isAccelerating)
		accelerationTimer = 1;
}

void AVehiclePawn::PerformAcceleration()
{
}

/*
*#####
*BRAKE
*#####
*/

void AVehiclePawn::Brake()
{
	networkComponent->SetDataIsBraking();
	isBraking = networkComponent->GetDataIsBraking();
	if (!isBraking)
	{
		brakeTimer = 1;
	}
}


void AVehiclePawn::PerformBraking(float& currentVelocity)
{
}

/*
*#####
*STEER
*#####
*/

void AVehiclePawn::Steer(float value)
{
	networkComponent->SetDataTurnValue(invertControls ? -value : value);
	steerValue = networkComponent->GetDataTurnValue();
}

void AVehiclePawn::PerformSteering(float currentVelocity, float currentAngular)
{
}

/*
*#####
*DRIFT
*#####
*/
void AVehiclePawn::Drift()
{
	networkComponent->SetDataIsDrifting();
	isDrifting = networkComponent->GetDataIsDrifting();

	if (isDrifting)
	{
		StartDrift();
	}
	else
	{
		StopDrift();
	}
}

void AVehiclePawn::PerformDrift(float currentVelocity, float currentAngular)
{
}

void AVehiclePawn::StartDrift()
{
	driftSign = FMath::Sign(steerValue);
	driftCameraRotationValue = -driftSign * cameraRotation;
	driftValue = initialDriftingRate;
	springArm->AddLocalRotation(FRotator(0, driftCameraRotationValue, 0));
}

void AVehiclePawn::StopDrift()
{
	isDrifting = false;
	driftIncreaseTimer = 0;
	driftDecreaseTimer = 0;
	springArm->AddLocalRotation(FRotator(0, -driftCameraRotationValue, 0));
	driftCameraRotationValue = 0;

	if (canDriftBoost)
	{
		canDriftBoost = false;

		for (AActor* particle : boostParticles)
		{
			particle->Destroy();
		}
		boostParticles.Empty();
		SetMaxSpeed(initialMaxSpeed * driftBoostRate);
		performDriftBoost = true;
	}
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
	inGround = true;
	terrainFriction = 1;
	for (UTyreComponent* tyre : tyres)
	{
		if (!tyre->SuspensionForce(suspensionDistance, suspensionRate, dampingRate, &terrainFriction))
			inGround = false;
	}
}

void AVehiclePawn::InstantiateDriftBoostParticles()
{
	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Rear"))
		{
			AActor* particle = GetWorld()->SpawnActor<AActor>(
				driftBoostParticle,
				tyre->GetRootPoint()->GetComponentLocation(),
				tyre->GetRootPoint()->GetComponentRotation()
			);
			particle->AttachToComponent(tyre->GetRootPoint(), FAttachmentTransformRules::KeepWorldTransform);
			boostParticles.Add(particle);
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
		networkComponent->SetUseObjectData(true);
		networkComponent->SetObjectData(-1);
	}
}

void AVehiclePawn::RemoveObject()
{
	if (currentObject)
	{
		currentObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		currentObject->SetOwner(nullptr);
		currentObject = nullptr;
		networkComponent->SetUseObjectData(false);
		networkComponent->SetObjectData(-1);
	}
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

FVector AVehiclePawn::CalculateAverageDirection(TArray<FVector> vectors)
{
	FVector forwardVector = FVector::ZeroVector;
	for (auto vector : vectors)
	{
		forwardVector += vector;
	}
	forwardVector /= vectors.Num();

	return forwardVector;
}

FVector AVehiclePawn::GetForward()
{
	TArray<FVector> frontArray, rearArray;
	for (UTyreComponent* tyre : tyres)
	{
		FVector tyrePosition = tyre->GetImpactPoint();
		if (tyrePosition != FVector::ZeroVector)
		{
			if (tyre->GetName().Contains("Front"))
			{
				frontArray.Add(tyrePosition);
			}
			else
			{
				rearArray.Add(tyrePosition);
			}
		}
	}
	FVector forward;
	if (rearArray.Num() != 0 && frontArray.Num() != 0)
	{
		FVector rearVector = CalculateAverageDirection(rearArray);
		FVector frontVector = CalculateAverageDirection(frontArray);
		forward = frontVector - rearVector;
		forward.Normalize();
	}
	else
	{
		forward = GetActorForwardVector();
	}
	// DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (forward * 3000), FColor::Green, false, -1,
	//               0, 10);
	//
	// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red,
	//                                  FString::Printf(
	// 	                                 TEXT("Forward= (%f,%f,%f)"),
	// 	                                 forward.X, forward.Y, forward.Z));
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

UNetworkComponent* AVehiclePawn::GetNetworkComponent() const
{
	return networkComponent;
}

AObjectBase* AVehiclePawn::GetCurrentObject() const
{
	return currentObject;
}

void AVehiclePawn::SetCurrentObject(TSubclassOf<UObject> CurrentObject)
{
	if (this->currentObject)
	{
		return;
	}
	if (CurrentObject)
	{
		networkComponent->SetUseObjectData(false);
		this->currentObject = GetWorld()->SpawnActor<AObjectBase>(CurrentObject,
		                                                          objectSpawnPoint->GetComponentLocation(),
		                                                          GetActorRotation());
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
	UE_LOG(LogTemp, Warning, TEXT("Current MAX Speed: %f"), maxSpeed)
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

UTextRenderComponent* AVehiclePawn::GetPlayerText()
{
	return playerNameText;
}

void AVehiclePawn::SetPing(int32 ping)
{
	return networkComponent->SetPing(ping);
}

int32 AVehiclePawn::GetPing()
{
	return networkComponent->GetPing();
}

bool AVehiclePawn::GetHasBeenHit() const
{
	return hasBeenHit;
}

bool AVehiclePawn::GetIsAccelerating() const
{
	return isAccelerating;
}

void AVehiclePawn::SetIsAccelerating(bool bIsAccelerating)
{
	isAccelerating = bIsAccelerating;
}

void AVehiclePawn::SetIsBraking(bool bIsBraking)
{
	isBraking = bIsBraking;
}

void AVehiclePawn::SetSteerValue(float bSteerValue)
{
	steerValue = bSteerValue;
}

void AVehiclePawn::SetIsDrifting(bool bIsDrifting)
{
	isDrifting = bIsDrifting;
}
