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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not found"))
	}

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / reverseRate;
	acceleration = maxSpeed;
	reverseAcceleration = acceleration / reverseRate;
	initialMaxSpeed = maxSpeed;
	decelerationTimer = 9999999;
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
	currentSpeed = (GetForward() * carMesh->GetPhysicsLinearVelocity()).Size();
	float currentAngular = (carMesh->GetPhysicsAngularVelocityInDegrees() * GetActorUpVector()).Size();
	if (inGround)
	{
		if (!invertControls && isAccelerating && !isBraking || invertControls && !isAccelerating && isBraking)
		{
			decelerationTimer = 1;
			PerformAcceleration();
		}
		else if (!invertControls && !isAccelerating && isBraking || invertControls && isAccelerating && !isBraking)
		{
			decelerationTimer = 1;
			PerformBraking(currentSpeed);
		}
		else
		{
			// currentSpeed = lastVelocity < currentSpeed ? -currentSpeed : currentSpeed;
			// carMesh->SetLinearDamping(1.f);
			brakeTimer = 0;
			accelerationTimer = 0;
			reverseTimer = 0;
			decelerationTimer += GetWorld()->DeltaTimeSeconds;
			float decelerationMultiplier = (isBraking && isAccelerating) ? 1 : 2.5f;


			if (currentSpeed > maxSpeed / 100)
				carMesh->AddForceAtLocation(
					GetForward() * acceleration * brakeRate /
					FMath::Exp(decelerationTimer / decelerationMultiplier),
					GetCenterOfMass());
		}
		// else
		// {
		// 	// carMesh->SetLinearDamping(0.5f);
		// 	brakeTimer = 0;
		// 	accelerationTimer = 0;
		// 	reverseTimer = 0;
		// 	deaccelerationTimer += GetWorld()->DeltaTimeSeconds;
		//
		// 	if (currentSpeed > maxSpeed / 100)
		// 		carMesh->AddForceAtLocation(
		// 			carMesh->GetForwardVector() * acceleration * accelerationRate / FMath::Exp(
		// 				deaccelerationTimer / 2.5f),
		// 			GetCenterOfMass());
		// 	// UE_LOG(LogTemp, Warning, TEXT("%f"), (acceleration * accelerationRate / FMath::Exp(deaccelerationTimer/10)))
		// }


		if (isDrifting)
		{
			PerformDrift(currentSpeed);
		}
		else
		{
			PerformSteering(currentSpeed, currentAngular);
		}

		// float rightFrictionForce = FVector::DotProduct(carMesh->GetPhysicsLinearVelocity(), GetActorRightVector());
		//
		// carMesh->AddForce(rightFrictionForce * GetActorRightVector() * -1 * frictionRate);
		//
		// DrawDebugLine(GetWorld(), GetActorLocation(),
		//               GetActorLocation() * rightFrictionForce * GetActorRightVector() * -1 * frictionRate,
		//               FColor::Green, false, -1, 0, 5);
		//
		// float frontFrictionForce = FVector::DotProduct(carMesh->GetPhysicsLinearVelocity(), GetActorForwardVector());
		//
		// carMesh->AddForce(frontFrictionForce * GetActorForwardVector() * -1 * frictionRate);
		//
	}
	else
	{
		carMesh->SetLinearDamping(.1f);
	}

	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Left"))
			tyre->RotateTyres(-currentSpeed);
		else
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
		DrawDebugLine(GetWorld(), GetActorLocation(),
			GetActorLocation() + GetForward() * 10000,
			FColor::Green, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), GetActorLocation(),
	GetActorLocation() + GetUp() * 10000,
	FColor::Red, false, -1, 0, 5);
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
		//                                  FString::Printf(
		// 	                                 TEXT("%s Position= (%f,%f,%f)"), *networkComponent->username,
		// 	                                 GetActorLocation().X, GetActorLocation().Y,
		// 	                                 GetActorLocation().Z));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Red,
		//                                  FString::Printf(
		// 	                                 TEXT("%s Rotation= (%f,%f,%f)"), *networkComponent->username,
		// 	                                 GetActorRotation().Pitch, GetActorRotation().Yaw,
		// 	                                 GetActorRotation().Roll));
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
		accelerationTimer = 0;
}

void AVehiclePawn::PerformAcceleration()
{
	carMesh->SetLinearDamping(1.f);
	if (currentSpeed < maxSpeed)
	{
		carMesh->AddForceAtLocation(
			GetForward() * acceleration * FMath::Exp(accelerationTimer) * accelerationRate,
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
	networkComponent->SetDataIsBraking();
	isBraking = networkComponent->GetDataIsBraking();
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
			-GetForward() * acceleration * FMath::Exp(reverseTimer) * accelerationRate,
			GetCenterOfMass());
		lastVelocity = currentVelocity;
		brakeTimer = 0;
		reverseTimer += GetWorld()->DeltaTimeSeconds;
	}
	else
	{
		carMesh->AddForceAtLocation(-GetForward() * acceleration * FMath::Exp(brakeTimer) * brakeRate,
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
	networkComponent->SetDataTurnValue(invertControls ? -value : value);
	steerValue = networkComponent->GetDataTurnValue();
}

float AVehiclePawn::CalculateMaxSteerValue(float currentVelocity)
{
	float x = currentVelocity / maxSpeed;
	float steeringAngle = maxSteerAngle;
	if (x < 0) steeringAngle *= FMath::Sin(-4 * x);
	else if (x < 0.25) steeringAngle *= FMath::Sin(1885 * x / 300);
	else steeringAngle *= FMath::Sin((x / 1.15) + 1.5);
	return steeringAngle;
}

void AVehiclePawn::PerformSteering(float currentVelocity, float currentAngular)
{
	if (turnTimer > turnToDriftSeconds)
	{
		UE_LOG(LogTemp, Error, TEXT("yepa"))
		Drift();
		return;
	}
	if (FMath::Abs(steerValue) > 0.75)
	{
		turnTimer += GetWorld()->DeltaTimeSeconds;
	}
	else
	{
		turnTimer = 0;
	}
	driftValue = 0;
	carMesh->SetAngularDamping(10.f);
	float steeringAngle = CalculateMaxSteerValue(currentVelocity);

	carMesh->SetPhysicsMaxAngularVelocityInDegrees(steeringAngle);

	carMesh->AddTorqueInDegrees(
		carMesh->GetUpVector() * steeringRate * 2 * steerValue * steeringAngle / maxDriftAngle,
		NAME_None, true);

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

	if (isDrifting)
	{
		driftSign = FMath::Sign(steerValue);
		driftCameraRotationValue = -driftSign * cameraRotation;
		springArm->AddLocalRotation(FRotator(0, driftCameraRotationValue, 0));
	}
	else
	{
		springArm->AddLocalRotation(FRotator(0, -driftCameraRotationValue, 0));
		driftCameraRotationValue = 0;
	}
}

void AVehiclePawn::PerformDrift(float currentVelocity)
{
	carMesh->SetAngularDamping(5.f);
	if (driftSign == 0 || FMath::Abs(currentVelocity) < maxSpeed / reverseRate || steerValue != 0 && driftSign !=
		FMath::Sign(steerValue) && turnTimer > turnToDriftSeconds)
	{
		isDrifting = false;
		turnTimer = 0;
		driftTimer = 0;
		driftInverseTimer = 0;
		springArm->AddLocalRotation(FRotator(0, -driftCameraRotationValue, 0));
		driftCameraRotationValue = 0;
		return;
	}

	carMesh->SetPhysicsMaxAngularVelocityInDegrees(CalculateMaxDriftValue(currentVelocity));

	carMesh->AddTorqueInDegrees(carMesh->GetUpVector() * steeringRate * driftSign, NAME_None, true);

	for (UTyreComponent* tyre : tyres)
	{
		if (tyre->GetName().Contains("Front"))
			tyre->Drift(steerValue);
	}
}

float AVehiclePawn::CalculateMaxDriftValue(float currentVelocity)
{
	float MaxSteerValue = CalculateMaxSteerValue(currentVelocity);
	if (driftValue == 0)
		driftValue = MaxSteerValue;

	if (steerValue == 0)
	{
		driftTimer = 0;
		driftInverseTimer = 0;
	}

	if (FMath::Sign(steerValue) == driftSign)
	{
		driftInverseTimer = 0;
		driftTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
		driftValue += (FMath::Exp(driftTimer / driftRateIncrease) - 1);
	}
	else
	{
		driftTimer = 0;
		driftInverseTimer += FMath::Abs(steerValue) * GetWorld()->DeltaTimeSeconds;
		driftValue -= (FMath::Exp(driftInverseTimer / driftRateDecrease) - 1);
	}

	return driftValue >= maxDriftAngle
		       ? maxDriftAngle
		       : driftValue <= MaxSteerValue
		       ? MaxSteerValue
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
		if (tyre->SuspensionForce(suspensionDistance, suspensionRate, dampingRate))
			inGround = true;
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
	currentObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	currentObject->SetOwner(nullptr);
	currentObject = nullptr;
	networkComponent->SetUseObjectData(false);
	networkComponent->SetObjectData(-1);
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

FVector AVehiclePawn::GetUp() const
{
	FHitResult hit;
	FVector position = GetActorLocation();
	FVector end = position - (GetActorUpVector() * 3000);
	FVector up = GetActorUpVector();

	GetWorld()->LineTraceSingleByChannel(hit, position, end, ECC_WorldStatic);

	if (hit.bBlockingHit)
	{
		up = hit.Normal;
	}

	return up;
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
		// this->currentObject->Destroy();
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
