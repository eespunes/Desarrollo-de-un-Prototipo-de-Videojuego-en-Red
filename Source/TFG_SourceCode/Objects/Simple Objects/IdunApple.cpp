// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "IdunApple.h"


#include "TFG_SourceCode/Objects/Base/HarmObjectBase.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

AIdunApple::AIdunApple()
{
	shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	shield->SetupAttachment(mesh);
}

void AIdunApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		if (timer >= boostDuration)
			Stop();
		timer += DeltaTime;
	}
}

void AIdunApple::UseObject()
{
	Super::UseObject();
	vehicle->SetMaxSpeedMultiplier(boostIncrease);
	shield->SetHiddenInGame(false);
	SendObjectEvent();
}

void AIdunApple::SetUp()
{
	objectType=4;
	Super::SetUp();
	shield->SetHiddenInGame(true);
	OnActorBeginOverlap.AddDynamic(this, &AIdunApple::OnOverlapBegin);
}

void AIdunApple::Stop()
{
	Super::Stop();
	vehicle->ResetMaxSpeedMultiplier();
	Destroy();
}

void AIdunApple::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	if (used)
	{
		AHarmObjectBase* object = Cast<AHarmObjectBase>(otherActor);
		if (object)
		{
			object->Destroy();
		}else
		{
			AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
			if (vehiclePawn)
			{
				if (used && vehiclePawn != vehicle)
				{
					vehiclePawn->Damage();
				}
			}
		}
	}
}
