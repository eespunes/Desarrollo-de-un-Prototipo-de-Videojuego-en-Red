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

	carMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh"));
	RootComponent = carMesh;

	objectSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Object Spawn Point"));
	objectSpawnPoint->SetupAttachment(carMesh);

	particleSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Spawn Point"));
	particleSpawnPoint->SetupAttachment(carMesh);

	normalSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Normal Spring Arm"));
	normalSpringArm->SetupAttachment(carMesh);

	normalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	normalCamera->SetupAttachment(normalSpringArm);
	normalCamera->Activate();

	playerNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player Name"));
	playerNameText->SetupAttachment(normalSpringArm);

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
	inForward = true;
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
		carMesh->SetLinearDamping(TNumericLimits<float>::Max());
		carMesh->SetAngularDamping(TNumericLimits<float>::Max());
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

	if (inGround)
	{
		if (!invertControls && isAccelerating && !isBraking || invertControls && !isAccelerating && isBraking)
		{
			PerformAcceleration();
		}
		else if (!invertControls && !isAccelerating && isBraking || invertControls && isAccelerating && !isBraking)
		{
			PerformBraking();
		}
		else
		{
			float exponentialValue = (FMath::Exp(decelerationTimer / decelerationRate) - 1);
			if (speed > 0)
			{
				speed = FMath::Max<float>(
					speed - maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);
			}
			else if (speed < 0)
			{
				speed = FMath::Min<float>(
					speed + maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);
			}
			decelerationTimer += GetWorld()->DeltaTimeSeconds;
		}


		if (isDrifting)
		{
			PerformDrift();
		}
		else
		{
			PerformSteering();
		}

		if (performDriftBoost)
		{
			if (performObjectBoost)
			{
				performDriftBoost = false;
				driftBoostTimer = 0;
			}
			else
			{
				if (driftBoostTimer > driftBoostTime)
				{
					performDriftBoost = false;
					maxSpeedMultiplier = 0;
					driftBoostTimer = 0;
				}
				else
				{
					driftBoostTimer += GetWorld()->DeltaTimeSeconds;
				}
			}
		}

		carMesh->SetPhysicsLinearVelocity(GetForward() * speed);
	}
	else
	{
		float exponentialValue = (FMath::Exp(decelerationTimer / (decelerationRate * flyRate)) - 1);
		if (speed > 0)
			speed = FMath::Max<float>(
				speed - maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);
		else if (speed < 0)
			speed = FMath::Min<float>(
				speed + maxSpeed * exponentialValue * GetWorld()->DeltaTimeSeconds, 0);

		carMesh->AddForce(GetForward() * speed / flyRate, NAME_None, true);
	}

	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Left"))
			tyre->RotateTyres(-speed / 2);
		else
			tyre->RotateTyres(speed / 2);
	}

	if (speed < 0)
	{
		if (!inReverse)
		{
			inForward = false;
			normalSpringArm->AddLocalRotation(FRotator(-20, 180, 0));
			inReverse = true;
		}
	}
	else
	{
		if (!inForward)
		{
			inReverse = false;
			normalSpringArm->AddLocalRotation(FRotator(-20, 180, 0));
			inForward = true;
		}
	}

	normalCamera->FieldOfView = constantFieldOfView + (variableFieldOfView * (FMath::Abs(speed) / maxSpeed));

	//DEBUG
	if (GEngine)
	{
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange,
		//                                  FString::Printf(
		// 	                                 TEXT("Steer: %f"), accelerationTimer));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange,
		//                                  FString::Printf(
		// 	                                 TEXT("Drift: %f"), driftValue));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Yellow,
		//                                  FString::Printf(
		// 	                                 TEXT("Speed: %f"), speed));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
		                                 FString::Printf(
			                                 TEXT("%s Position= (%f,%f,%f)"), *networkComponent->username,
			                                 GetActorLocation().X, GetActorLocation().Y,
			                                 GetActorLocation().Z));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("%s Rotation= (%f,%f,%f)"), *networkComponent->username,
			                                 GetActorRotation().Pitch,
			                                 GetActorRotation().Yaw,
			                                 GetActorRotation().Roll));
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
	if (!hasBeenHit)
		decelerationTimer = 1;

	if (maxSpeedMultiplier == 1)
	{
		speed = FMath::Min<float>(
			speed + maxSpeed * (FMath::Exp(accelerationTimer / accelerationRate) - 1) * GetWorld()->DeltaTimeSeconds,
			maxSpeed);
	}
	else
	{
		if (maxSpeedMultiplier < 1 && speed > maxSpeed * maxSpeedMultiplier)
			speed = FMath::Max<float>(
				speed - maxSpeed * (FMath::Exp(accelerationTimer / accelerationRate) - 1) * GetWorld()->
				DeltaTimeSeconds,
				maxSpeed * maxSpeedMultiplier);
		else
			speed = FMath::Min<float>(
				speed + maxSpeed * (FMath::Exp(accelerationTimer / accelerationRate) - 1) * GetWorld()->
				DeltaTimeSeconds,
				maxSpeed * maxSpeedMultiplier);
	}

	if (speed == maxSpeed * maxSpeedMultiplier)
	{
		accelerationTimer = 1;
	}
	else
	{
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
	networkComponent->SetDataIsBraking();
	isBraking = networkComponent->GetDataIsBraking();
	if (!isBraking)
	{
		brakeTimer = 1;
	}
}


void AVehiclePawn::PerformBraking()
{
	if (!hasBeenHit)
		decelerationTimer = 1;

	speed = FMath::Max<float>(
		speed - maxSpeed * (FMath::Exp(brakeTimer / brakeRate) - 1) * GetWorld()->DeltaTimeSeconds,
		reverseSpeed * maxSpeedMultiplier);
	brakeTimer += GetWorld()->DeltaTimeSeconds;
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

void AVehiclePawn::PerformSteering()
{
	GetMesh()->AddLocalRotation(
		FRotator(
			0,
			steeringRate * steerValue * (speed / (maxSpeed * maxSpeedMultiplier)) * GetWorld()->DeltaTimeSeconds,
			0));
	for (UTyreComponent* tyre : tyres)
	{
		if (traction4x4 || tyre->GetName().Contains("Front"))
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
	networkComponent->SetDataIsDrifting();
	isDrifting = networkComponent->GetDataIsDrifting();

	if (isDrifting) StartDrift();
	else StopDrift();
}

void AVehiclePawn::PerformDrift()
{
	if (speed < maxSpeed / 2)
	{
		StopDrift();
		return;
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

	if (driftValue >= finalDriftingRate / 2)
	{
		if (!canDriftBoost)
		{
			InstantiateDriftBoostParticles();
			canDriftBoost = true;
		}
	}

	for (UTyreComponent* tyre : tyres)
	{
		if (traction4x4 || tyre->GetName().Contains("Front"))
			tyre->Drift(steerValue);
	}
}

void AVehiclePawn::StartDrift()
{
	driftSign = FMath::Sign(steerValue);
	driftCameraRotationValue = -driftSign * cameraRotation;
	driftValue = initialDriftingRate;
	normalSpringArm->AddLocalRotation(FRotator(0, driftCameraRotationValue, 0));
}

void AVehiclePawn::StopDrift()
{
	isDrifting = false;
	driftIncreaseTimer = 0;
	driftDecreaseTimer = 0;
	normalSpringArm->AddLocalRotation(FRotator(0, -driftCameraRotationValue, 0));
	driftCameraRotationValue = 0;

	if (canDriftBoost)
	{
		canDriftBoost = false;

		maxSpeedMultiplier = driftBoostRate;

		for (AActor* particle : boostParticles)
		{
			particle->Destroy();
		}
		boostParticles.Empty();

		performDriftBoost = true;
	}
}

/*
*#######
*PHYSICS
*#######
*/

void AVehiclePawn::GravityForce()
{
	carMesh->AddForce(FVector::CrossProduct(GetForward(), GetActorRightVector()) * GetWorld()->GetGravityZ(), NAME_None,
	                  true);
}

void AVehiclePawn::SuspensionForces()
{
	inGround = true;
	float terrainFriction = 1;
	for (UTyreComponent* tyre : tyres)
	{
		if (!tyre->SuspensionForce(suspensionDistance, suspensionRate, dampingRate, &terrainFriction))
			inGround = false;
	}
	if (!performObjectBoost && !performDriftBoost && !hasBeenHit)
	{
		maxSpeedMultiplier = terrainFriction;
	}
}

/*
*####### #######
*OBJECTS RELATED
*####### #######
*/

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
	maxSpeedMultiplier = 0;
	accelerationTimer = accelerationRate;
	decelerationTimer = decelerationRate;
	brakeTimer = brakeRate;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartMatineeCameraShake(cameraShake, 1.0f);
}

void AVehiclePawn::WaitAfterHit(float DeltaTime)
{
	if (hasBeenHit)
	{
		if (hitTimer >= hitWaiting)
		{
			hasBeenHit = false;
			maxSpeedMultiplier = 1;
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
	if (rearArray.Num() != 0 && frontArray.Num() != 0)
	{
		FVector rearVector = CalculateAverageDirection(rearArray);
		FVector frontVector = CalculateAverageDirection(frontArray);
		forward = frontVector - rearVector;
		forward.Normalize();
	}
	// else
	// {
	// 	forward = GetActorForwardVector();
	// }
	return forward;
}

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

float AVehiclePawn::GetSpeed()
{
	return speed;
}

void AVehiclePawn::SetSpeed(float speedField)
{
	speed = speedField;
}

float AVehiclePawn::GetMaxSpeed() const
{
	return maxSpeed;
}

void AVehiclePawn::SetMaxSpeedMultiplier(float multiplier)
{
	maxSpeedMultiplier = multiplier;
	performObjectBoost = true;
}

void AVehiclePawn::ResetMaxSpeedMultiplier()
{
	maxSpeedMultiplier = 1;
	performObjectBoost = false;
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

bool AVehiclePawn::GetIsBraking()
{
	return isBraking;
}

void AVehiclePawn::SetIsBraking(bool bIsBraking)
{
	isBraking = bIsBraking;
}

void AVehiclePawn::SetSteerValue(float bSteerValue)
{
	steerValue = bSteerValue;
}

bool AVehiclePawn::GetIsDrifting() const
{
	return isDrifting;
}

void AVehiclePawn::SetIsDrifting(bool bIsDrifting)
{
	isDrifting = bIsDrifting;
}

float AVehiclePawn::GetDriftValue()
{
	return driftValue;
}

void AVehiclePawn::SetDriftValue(float driftValueField)
{
	driftValue = driftValueField;
}

float AVehiclePawn::GetAccelerationTimer()
{
	return accelerationTimer;
}

void AVehiclePawn::SetAccelerationTimer(float accelerationTimerField)
{
	accelerationTimer = accelerationTimerField;
}

float AVehiclePawn::GetBrakeTimer()
{
	return brakeTimer;
}

void AVehiclePawn::SetBrakeTimer(float brakeTimerField)
{
	brakeTimer = brakeTimerField;
}

float AVehiclePawn::GetDecelerationTimer()
{
	return decelerationTimer;
}

void AVehiclePawn::SetDecelerationTimer(float decelerationTimerField)
{
	decelerationTimer = decelerationTimerField;
}

float AVehiclePawn::GetDriftIncreaseTimer()
{
	return driftIncreaseTimer;
}

void AVehiclePawn::SetDriftIncreaseTimer(float driftIncreaseTimerField)
{
	driftIncreaseTimer = driftIncreaseTimer;
}

float AVehiclePawn::GetDriftDecreaseTimer()
{
	return driftDecreaseTimer;
}

void AVehiclePawn::SetDriftDecreaseIncreaseTimer(double driftDecreaseTimerField)
{
	driftDecreaseTimer = driftDecreaseTimerField;
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
