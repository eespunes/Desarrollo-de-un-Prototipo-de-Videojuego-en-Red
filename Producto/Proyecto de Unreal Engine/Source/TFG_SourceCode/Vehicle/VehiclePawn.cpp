// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"

#include <iterator>


#include "DrawDebugHelpers.h"
#include "FramePro/FramePro.h"
#include "Math/UnitConversion.h"
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

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(mesh);

	raceComponent = CreateDefaultSubobject<URaceComponent>(TEXT("Race Component"));
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	lastUpVector = GetActorUpVector();

	reverseSpeed = -maxSpeed / reverseRate;
	acceleration = (maxSpeed / accelerationRate) * 200;
	initialMaxSpeed = maxSpeed;
}

// Called every frame
void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!canUseObject)
	{
		if(hiTimer>=hitWaiting)
		{
			canUseObject = true;
			maxSpeed = initialMaxSpeed;
			hiTimer=0;
		}
		hiTimer+=DeltaTime;
	}

	GravityForce();
	SuspensionForces();
	Movement();
}

void AVehiclePawn::Accelerate()
{
	isAccelerating = !isAccelerating;
}

void AVehiclePawn::Brake()
{
	isBraking = !isBraking;
}

void AVehiclePawn::Turn(float value)
{
	// UE_LOG(LogTemp,Error,TEXT("Turn Value= %f"),value);
	turnValue = value;
}

void AVehiclePawn::Drift()
{
	if (turnTimer < turnToDriftSeconds)
	{
		isDrifting = !isDrifting;
		if (isDrifting)
			driftSign = FMath::Sign(turnValue);;
	}
}

void AVehiclePawn::UseObject()
{
	if (currentObject && canUseObject)
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

void AVehiclePawn::Movement()
{
	float deltaTime = GetWorld()->DeltaTimeSeconds;
	float currentVelocity = (GetActorForwardVector() * mesh->GetPhysicsLinearVelocity()).Size();
	float currentAngular = (mesh->GetPhysicsAngularVelocityInDegrees() * GetActorUpVector()).Size();

	FString action;
	if (inGround)
	{
		if (isAccelerating & !isBraking)
		{
			mesh->SetLinearDamping(1.5f);
			if (currentVelocity < maxSpeed)
			{
				mesh->AddForceAtLocation(GetActorForwardVector() * acceleration, GetCenterOfMass());
				lastVelocity = currentVelocity;
			}
			action = TEXT("Accelerating");
		}
		else if (!isAccelerating & isBraking)
		{
			mesh->SetLinearDamping(1.5f);
			currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
			if (currentVelocity > reverseSpeed)
			{
				mesh->AddForceAtLocation(-GetActorForwardVector() * acceleration * brakeRate, GetCenterOfMass());
				lastVelocity = currentVelocity;
			}

			action = TEXT("Braking");
		}
		else if (isBraking && isAccelerating)
		{
			currentVelocity = lastVelocity < currentVelocity ? -currentVelocity : currentVelocity;
			mesh->SetLinearDamping(2.5f);
			action = TEXT("Braking and Accelerating");
		}
		else
		{
			mesh->SetLinearDamping(1.f);
			action = TEXT("Nothing");
		}

		if (isDrifting)
		{
			if (driftSign == 0 || FMath::Abs(currentVelocity) < maxSpeed / reverseRate)
			{
				isDrifting = false;
				turnTimer = 0;
				return;
			}
			action += TEXT(" - Drift");
			action += driftSign > 0 ? TEXT(" - Right") : TEXT(" - Left");
			mesh->SetPhysicsMaxAngularVelocityInDegrees(CalculateMaxDriftValue());

			if (turnTimer >= turnToDriftSeconds && FMath::Abs(turnValue) <= 0)
			{
				isDrifting = false;
				turnTimer = 0;
			}

			mesh->AddTorqueInDegrees(GetActorUpVector() * steeringRate * driftSign, NAME_None, true);
		}
		else
		{
			driftValue = 0;
			mesh->SetPhysicsMaxAngularVelocityInDegrees(
				(maxTurnAngle + (maxDriftAngle - maxTurnAngle) * (1 - (currentVelocity / maxSpeed))));
			if (FMath::Abs(turnValue) == 0 /*|| (currentVelocity / maxSpeed) < frictionDecelerationRate*/)
			{
				action += TEXT(" - No Turn");
				if (currentAngular > maxTurnAngle * frictionDecelerationRate)
				{
					action += TEXT(" - Stopping");
					// mesh->AddTorqueInDegrees(GetActorUpVector() * lastTurnValue * -currentAngular * 10, NAME_None, true);
				}
				else
				{
					action += TEXT(" - Stopped");
					lastTurnValue = 0;
				}
			}
			else
			{
				action += TEXT(" - Turn");
				if (FMath::Abs(turnValue) >= 0)
					turnTimer += GetWorld()->DeltaTimeSeconds;
				else
				{
					turnTimer = 0;
				}
				if (turnTimer >= turnToDriftSeconds)
				{
					driftSign = FMath::Sign(turnValue);
					isDrifting = true;
				}
				else
				{
					mesh->AddTorqueInDegrees(GetActorUpVector() * steeringRate * turnValue * currentVelocity / maxSpeed,
					                         NAME_None, true);
					lastTurnValue = FMath::Sign(turnValue);
				}
			}
		}
	}

	//DEBUG
	if (GEngine)
	{
		// 	GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
		// 		                                 TEXT("Angular: %f"), currentAngular));
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Yellow,
		                                 FString::Printf(
			                                 TEXT("Speed: %f"), currentVelocity));
		// 	GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue, FString::Printf(TEXT("%s"), *action));
		GEngine->AddOnScreenDebugMessage(-1, deltaTime, FColor::Blue,
		                                 FString::Printf(
			                                 TEXT("Object= %s"),
			                                 currentObject ? *currentObject->GetName() : *FString("NO OBJECT")));
	}
}

void AVehiclePawn::GravityForce() const
{
	mesh->AddForce(GetActorUpVector() * GetWorld()->GetGravityZ(), NAME_None, true);
}


FVector AVehiclePawn::GetCenterOfMass() const
{
	return mesh->GetCenterOfMass() - GetActorUpVector() * 10 +
		GetActorForwardVector() * 10;
}

void AVehiclePawn::SuspensionForces()
{
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
	//SPAWN OBJECT AT THE DESIRED POSITION

	this->currentObject = CurrentObject;
	this->currentObject->SetVehicle(this);
	this->currentObject->SetOwner(this);
	this->currentObject->SetActorLocation(objectSpawnPoint->GetComponentLocation());
	this->currentObject->AttachToComponent(objectSpawnPoint, FAttachmentTransformRules::KeepWorldTransform);
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

float AVehiclePawn::GetInitialMaxSpeed()
{
	return initialMaxSpeed;
}

void AVehiclePawn::Damage()
{
	canUseObject = false;
	maxSpeed = 0;
}
