#include "RacingState.h"


#include "DrawDebugHelpers.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/AI/AIVehicleController.h"

RacingState::RacingState(AAIVehicleController* VehicleController): State(VehicleController)
{
	SetUp();
}

void RacingState::SetUp()
{
	State::SetUp();
	UE_LOG(LogTemp, Warning, TEXT("RACING"));
	vehicle = vehicleController->GetVehicle();
}

void RacingState::Steering()
{
	ACheckPoint* currentCheckpoint = vehicleController->GetCurrentCheckpoint();
	if (currentCheckpoint)
	{
		FVector checkpointLocation = currentCheckpoint->GetActorLocation();
		FVector vehicleLocation = vehicle->GetActorLocation();

		FVector direction = checkpointLocation - vehicleLocation;
		direction.Normalize();

		FVector forward = vehicle->GetForward();

		float angle = FVector::DotProduct(vehicle->GetActorRightVector(), direction);

		DrawDebugLine(vehicle->GetWorld(), vehicleLocation, vehicleLocation + direction * 10000, FColor::Purple, false,
		              -1,
		              0, 5);
		DrawDebugLine(vehicle->GetWorld(), vehicleLocation, vehicleLocation + vehicle->GetActorForwardVector() * 1000,
		              FColor::Blue, false, -1,
		              0, 5);

		vehicle->Steer(angle);
		if (FMath::Abs(angle) > 0.5f && !vehicle->GetDrifting())
		{
			vehicle->Drift();
		}
		else if (vehicle->GetDrifting())
		{
			vehicle->Drift();
		}
	}
}

void RacingState::Update()
{
	State::Update();
	Steering();
	if (!vehicle->GetIsAccelerating())
		vehicle->Accelerate();
}

void RacingState::Exit()
{
	State::Exit();
}
