// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "AIVehicleController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "States/IdleState.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/IVehicle.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

AAIVehicleController::~AAIVehicleController()
{
	delete currentState;
}

void AAIVehicleController::BeginPlay()
{
	Super::BeginPlay();

	FindCheckpoints();

	vehicle = Cast<AVehiclePawn>(GetOwner());

	currentState = new IdleState(this);
}

void AAIVehicleController::FindCheckpoints()
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
	GetBlackboardComponent()->SetValueAsVector(TEXT("NextCheckpointPosition"), checkpoints[0]->GetActorLocation());
}

void AAIVehicleController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	currentState->Update();
}


void AAIVehicleController::SetCurrentState(State* CurrentState)
{
	currentState = CurrentState;
}

ACheckPoint* AAIVehicleController::GetCurrentCheckpoint()
{
	return checkpoints[0];
}

IVehicle* AAIVehicleController::GetVehicle()
{
	return vehicle;
}
