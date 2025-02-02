// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ObjectBase.h"

#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

// Sets default values
AObjectBase::AObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

void AObjectBase::SetUp()
{
	used = false;

	if (vehicle)
	{
		gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (gameInstance->IsMultiplayer())
		{
			parentUsername = vehicle->GetNetworkComponent()->GetUsername();
			isSender = gameInstance->GetPlayer().pla_username == parentUsername;
			if (id < 0)
				id = gameInstance->GetNextObjectID();
			gameInstance->AddObjectInGame(this);
			SendObjectEvent();
		}
	}
}

void AObjectBase::Stop()
{
	vehicle->RemoveObject();
	gameInstance->RemoveObjectInGame(this);
}

void AObjectBase::SendObjectEvent()
{
	if (isSender)
	{
		FString json;
		FObjectStruct* objectData = new FObjectStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->used=used;
		FJsonObjectConverter::UStructToJsonObjectString<FObjectStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
}

// Called every frame
void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!used)
	{
		AddActorLocalRotation(FQuat(FRotator(0, 0.5f, 0)), false, 0, ETeleportType::None);
	}
}

void AObjectBase::UseObject()
{
	used = true;
}

void AObjectBase::SetVehicle(AVehiclePawn* VehiclePawn)
{
	vehicle = VehiclePawn;
	SetUp();
}

bool AObjectBase::IsUsed()
{
	return used;
}

bool AObjectBase::IsSender()
{
	return isSender;
}

void AObjectBase::SetID(int32 ID)
{
	id = ID;
}

void AObjectBase::SetType(int32 type)
{
	objectType = type;
}

int32 AObjectBase::GetID()
{
	return id;
}

FString AObjectBase::GetUsername()
{
	return parentUsername;
}
