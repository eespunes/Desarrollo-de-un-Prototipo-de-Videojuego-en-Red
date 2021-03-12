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
	// UE_LOG(LogTemp, Warning, TEXT("RACING"));
	vehicle = vehicleController->GetVehicle();
}

void RacingState::Steering()
{
	ACheckPoint* currentCheckpoint = vehicleController->GetCurrentCheckpoint();
	if (currentCheckpoint)
	{
		if (lastCheckpoint != currentCheckpoint)
		{
			checkpointLocation = currentCheckpoint->GetLocation();
			// UE_LOG(LogTemp, Warning, TEXT("Racing Line (%f, %f, %f)"), checkpointLocation.X, checkpointLocation.Y,
			//        checkpointLocation.Z);
			lastCheckpoint = currentCheckpoint;
		}
		FVector vehicleLocation = vehicle->GetActorLocation();

		FVector direction = checkpointLocation - vehicleLocation;
		direction.Normalize();

		FVector forward = vehicle->GetForward();

		float angle = FVector::DotProduct(vehicle->GetActorRightVector(), direction);

		// GEngine->AddOnScreenDebugMessage(-1, vehicle->GetWorld()->DeltaTimeSeconds, FColor::Yellow,
  //                                FString::Printf(
  //                                    TEXT("%s Angle: %f"), *vehicle->GetOwner()->GetActorLabel(),angle));

		if (vehicle->GetCurrentSpeed() < 1 && started)
		{
			vehicle->Steer(-angle);

			if (vehicle->GetIsAccelerating())
				vehicle->Accelerate();

			if (!vehicle->GetBraking())
				vehicle->Brake();

			if (FMath::Abs(angle) > 0.25f)
			{
				vehicle->Brake();
				vehicle->Accelerate();
			}
		}
		else
		{
			vehicle->Steer(angle);
			if (FMath::Abs(angle) > 0.5f)
			{
				if (FMath::Abs(angle) > 0.75f)
				{
					if (vehicle->GetIsAccelerating() && vehicle->GetCurrentSpeed() > vehicle->GetMaxSpeed() * 0.25f)
						vehicle->Accelerate();
					else if (!vehicle->GetIsAccelerating())
						vehicle->Accelerate();
				}
				else if (!vehicle->GetIsAccelerating() && vehicle->GetCurrentSpeed() < vehicle->GetMaxSpeed() * 0.25f)
				{
					vehicle->Accelerate();
				}
				if (!vehicle->GetDrifting())
					vehicle->Drift();
				else if (vehicle->GetDriftSign() > 0 && angle < 0 || vehicle->GetDriftSign() < 0 && angle > 0)
					vehicle->Drift();
			}
			else
			{
				if (!vehicle->GetIsAccelerating())
				{
					vehicle->Accelerate();
				}
				if (vehicle->GetDrifting())
					vehicle->Drift();
			}
		}

		DrawDebugLine(vehicle->GetWorld(), vehicleLocation, vehicleLocation + direction * 10000, FColor::Purple, false,
		              -1,
		              0, 5);
		DrawDebugLine(vehicle->GetWorld(), vehicleLocation, vehicleLocation + vehicle->GetActorForwardVector() * 1000,
		              FColor::Blue, false, -1,
		              0, 5);
	}
}

void RacingState::Update()
{
	State::Update();
	Steering();

	if (!started)
	{
		startTimer += vehicle->GetWorld()->GetDeltaSeconds();
		if (startTimer >= 2)
		{
			startTimer = 0;
			started = true;
		}
	}
}

void RacingState::Exit()
{
	State::Exit();
}
