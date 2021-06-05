// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ObjectContainer.h"

#include <string>
#include <fstream>
#include <iostream>

#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/NetworkComponent.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"
#include "TFG_SourceCode/Vehicle/Controllers/VehicleNetworkController.h"


// Sets default values
AObjectContainer::AObjectContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis Mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AObjectContainer::BeginPlay()
{
	Super::BeginPlay();
	enabled = true;
	gameInstance = Cast<URaceGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	OnActorBeginOverlap.AddDynamic(this, &AObjectContainer::OnOverlapBegin);
}

void AObjectContainer::DisableContainer()
{
	enabled = false;
	SetActorHiddenInGame(true);
}

// Called every frame
void AObjectContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FQuat(FRotator(0, 1, 0)), false, 0, ETeleportType::None);
	if (!enabled)
	{
		if (timer >= timeToEnable)
		{
			enabled = true;
			SetActorHiddenInGame(false);
			timer = 0;
		}
		timer += DeltaTime;
	}
}

void AObjectContainer::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	if (enabled)
	{
		AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
		if (vehiclePawn)
		{
			if (!Cast<AObjectBase>(vehiclePawn->GetCurrentObject()))
			{
				if (!Cast<AVehicleNetworkController>(vehiclePawn->GetController()))
				{
					int32 objectIdx = gameInstance->GetRandomObject(vehiclePawn->GetRaceComponent()->GetPosition());
				
					vehiclePawn->GetNetworkComponent()->SetObjectData(objectIdx);
					
					vehiclePawn->SetCurrentObject(gameInstance->GetObject(objectIdx));
				}
				DisableContainer();
			}
		}
	}
}
