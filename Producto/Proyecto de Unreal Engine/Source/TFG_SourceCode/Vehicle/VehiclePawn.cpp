// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"


#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/NetworkComponent.h"
#include "Components/RaceComponent.h"
#include "Components/TyreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"

// Sets default values
AVehiclePawn::AVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	RootComponent = mesh;

	objectSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Object Spawn Point"));
	objectSpawnPoint->SetupAttachment(mesh);

	particleSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Particle Spawn Point"));
	particleSpawnPoint->SetupAttachment(mesh);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(mesh);

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
	acceleration = (maxSpeed / accelerationRate) * 200;
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
		mesh->SetLinearDamping(1000000);
		mesh->SetAngularDamping(1000000);
	}
}

/*
*########
*MOVEMENT
*########
*/

void AVehiclePawn::Movement()
{
	currentSpeed = (GetActorForwardVector() * mesh->GetPhysicsLinearVelocity()).Size();
	float currentAngular = (mesh->GetPhysicsAngularVelocityInDegrees() * GetActorUpVector()).Size();
	if (inGround)
	{
		if (!invertControls && isAccelerating && !isBraking || invertControls && !isAccelerating && isBraking)
		{
			PerformAcceleration();
		}
		else if (!invertControls && !isAccelerating && isBraking || invertControls && isAccelerating && !isBraking)
		{
			PerformBraking(currentSpeed);
		}
		else if (isBraking && isAccelerating)
		{
			currentSpeed = lastVelocity < currentSpeed ? -currentSpeed : currentSpeed;
			mesh->SetLinearDamping(2.5f);
		}
		else
		{
			mesh->SetLinearDamping(1.f);
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
		mesh->SetLinearDamping(.01f);
	}

	for (UTyreComponent* tyre : tyres)
	{
		tyre->RotateTyres(currentSpeed/*,maxSpeed*/);
	}

	//DEBUG
	if (GEngine)
	{
		// 	GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
		// 		                                 TEXT("Angular: %f"), currentAngular));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Yellow,
		//                                  FString::Printf(
		// 	                                 TEXT("Speed: %f"), currentSpeed));
		// GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue,
		//                                  FString::Printf(
		// 	                                 TEXT("Object= %s"),
		// 	                                 currentObject ? *currentObject->GetName() : *FString("NO OBJECT")));
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
}

void AVehiclePawn::PerformAcceleration()
{
	mesh->SetLinearDamping(1.5f);
	if (currentSpeed < maxSpeed)
	{
		mesh->AddForceAtLocation(GetActorForwardVector() * acceleration, GetCenterOfMass());
		lastVelocity = currentSpeed;
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
}

void AVehiclePawn::PerformBraking(float& currentVelocity)
{
	mesh->SetLinearDamping(1.5f);
	currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
	if (currentVelocity > reverseSpeed)
	{
		mesh->AddForceAtLocation(-GetActorForwardVector() * acceleration * brakeRate, GetCenterOfMass());
		lastVelocity = currentVelocity;
	}
}

/*
*#####
*STEER
*#####
*/

void AVehiclePawn::Steer(float value)
{
	turnValue = invertControls ? -value : value;
}

void AVehiclePawn::PerformSteering(float currentVelocity, float currentAngular)
{
	driftValue = 0;
	mesh->SetAngularDamping(2);
	mesh->SetPhysicsMaxAngularVelocityInDegrees(
		(maxTurnAngle + (maxDriftAngle - maxTurnAngle) * (1 - (currentVelocity / maxSpeed))));
	if (FMath::Abs(turnValue) == 0)
	{
		if (currentAngular <= maxTurnAngle * frictionDecelerationRate)
		{
			lastTurnValue = 0;
		}
	}
	else
	{
		mesh->AddTorqueInDegrees(GetActorUpVector() * steeringRate * turnValue * currentVelocity / maxSpeed,
		                         NAME_None, true);
		lastTurnValue = FMath::Sign(turnValue);
	}
	for (UTyreComponent* tyre : tyres)
	{
		tyre->Steer(turnValue);
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
			driftSign = FMath::Sign(turnValue);;
	}
	for (UTyreComponent* tyre : tyres)
	{
		tyre->Drift(turnValue);
	}
}

void AVehiclePawn::PerformDrift(float currentVelocity)
{
	mesh->SetAngularDamping(2);
	if (driftSign == 0 || FMath::Abs(currentVelocity) < maxSpeed / reverseRate)
	{
		isDrifting = false;
		turnTimer = 0;
		return;
	}
	mesh->SetPhysicsMaxAngularVelocityInDegrees(CalculateMaxDriftValue());

	if (turnTimer >= turnToDriftSeconds && FMath::Abs(turnValue) <= 0)
	{
		isDrifting = false;
		turnTimer = 0;
	}

	mesh->AddTorqueInDegrees(GetActorUpVector() * steeringRate * driftSign, NAME_None, true);
}

float AVehiclePawn::CalculateMaxDriftValue()
{
	if (driftValue == 0)
		driftValue = maxTurnAngle;

	if (FMath::Abs(turnValue) >= 1 && FMath::Sign(turnValue) == driftSign)
	{
		driftTimer += GetWorld()->DeltaTimeSeconds;
		driftValue += FMath::Exp(driftTimer - 4);
	}
	else
		driftTimer = 0;

	return driftValue >= maxDriftAngle ? maxDriftAngle : driftSign * driftValue;
}

/*
*#######
*PHYSICS
*#######
*/

void AVehiclePawn::GravityForce() const
{
	mesh->AddForce(GetActorUpVector() * GetWorld()->GetGravityZ(), NAME_None, true);
}

void AVehiclePawn::SuspensionForces()
{
	inGround = true;
	for (UTyreComponent* tyre : tyres)
	{
		bool temp = tyre->SuspensionForce(suspensionDistance, suspensionRate, dampingRate);
		if (!temp)
		{
			inGround = false;
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
	return mesh;
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
	return mesh->GetCenterOfMass() - GetActorUpVector() * 10 +
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
