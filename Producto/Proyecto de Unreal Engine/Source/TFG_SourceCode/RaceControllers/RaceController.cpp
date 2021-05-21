// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "RaceController.h"


#include "CheckPoint.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"


// Sets default values
ARaceController::ARaceController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARaceController::BeginPlay()
{
	Super::BeginPlay();
	FindCheckpoints();
	FindVehicles();

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		raceStarts.Add(*It);
	}
	APlayerStart* aux;
	for (int i = 1; i < raceStarts.Num(); i++)
	{
		for (int j = raceStarts.Num() - 1; j >= i; j--)
		{
			if (raceStarts[j]->GetName() < raceStarts[j - 1]->GetName())
			{
				aux = raceStarts[j];
				raceStarts[j] = raceStarts[j - 1];
				raceStarts[j - 1] = aux;
			}
		}
	}
}

void ARaceController::FindCheckpoints()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		ACheckPoint* checkpoint = Cast<ACheckPoint>(actor);
		if (checkpoint)
		{
			checkpoints.Add(checkpoint);
		}
	}

	ACheckPoint* aux;
	for (int i = 1; i < checkpoints.Num(); i++)
	{
		for (int j = checkpoints.Num() - 1; j >= i; j--)
		{
			if (checkpoints[j]->GetPosition() < checkpoints[j - 1]->GetPosition())
			{
				aux = checkpoints[j];
				checkpoints[j] = checkpoints[j - 1];
				checkpoints[j - 1] = aux;
			}
		}
	}
}

void ARaceController::FindVehicles()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehiclePawn::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		AVehiclePawn* vehicle = Cast<AVehiclePawn>(actor);
		if (vehicle)
		{
			vehicles.Add(vehicle->GetRaceComponent());
		}
	}
}

ACheckPoint* ARaceController::GetCheckpoint(int32 idx)
{
	if (idx < checkpoints.Num())
		return checkpoints[idx];
	return nullptr;
}

int ARaceController::GetNumberOfCheckpoints() const
{
	return checkpoints.Num();
}

int32 ARaceController::GetNumberOfVehicles() const
{
	return vehicles.Num();
}

URaceComponent* ARaceController::GetVehicle(int32 position)
{
	if (position >= vehicles.Num() || position < 0)
		return nullptr;
	return vehicles[position];
}

bool ARaceController::GetCanRace() const
{
	return canRace;
}

void ARaceController::SetCanRace(bool value)
{
	canRace = value;
}

APlayerStart* ARaceController::GetRaceStart(int idx)
{
	if (idx < raceStarts.Num())
		return raceStarts[idx];
	return nullptr;
}

void ARaceController::UpdateTable()
{
	if (vehicles.Num() == 1)
	{
		vehicles[0]->SetPosition(1);
		return;
	}
	
	for (URaceComponent* raceComponent : vehicles)
	{
		raceComponent->CalculateTimeValue();
	}

	URaceComponent* aux;
	for (int i = 1; i < vehicles.Num(); i++)
	{
		for (int j = vehicles.Num() - 1; j >= i; j--)
		{
			if (vehicles[j]->GetTimeValue() > vehicles[j - 1]->GetTimeValue())
			{
				aux = vehicles[j];
				vehicles[j] = vehicles[j - 1];
				vehicles[j - 1] = aux;
			}
			else if (vehicles[j]->GetTimeValue() == vehicles[j - 1]->GetTimeValue())
			{
				if (vehicles[j]->GetDistance() < vehicles[j - 1]->GetDistance())
				{
					aux = vehicles[j];
					vehicles[j] = vehicles[j - 1];
					vehicles[j - 1] = aux;
				}
			}
		}
	}

	for (int i = 0; i < vehicles.Num(); i++)
	{
		vehicles[i]->SetPosition(i + 1);
	}
}

// Called every frame
void ARaceController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTable();
}
