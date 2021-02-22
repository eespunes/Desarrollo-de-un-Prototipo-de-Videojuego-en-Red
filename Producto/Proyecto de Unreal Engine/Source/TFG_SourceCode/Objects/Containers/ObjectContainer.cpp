// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ObjectContainer.h"

#include <string>
#include <fstream>
#include <iostream>

#include "TFG_SourceCode/Vehicle/VehiclePawn.h"


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
				vehiclePawn->SetCurrentObject(GetObject());
				DisableContainer();
			}
		}
	}
}


AObjectBase* AObjectContainer::GetObject()
{
	int32 RandRange = FMath::RandRange(0, objects.Num() - 1);
	UE_LOG(LogTemp, Warning, TEXT("%i"), RandRange);
	return GetWorld()->SpawnActor<AObjectBase>(objects[RandRange]->GeneratedClass, GetActorLocation(),
	                                           GetActorRotation());
}
