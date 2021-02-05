#include "RacingState.h"


#include "DrawDebugHelpers.h"
#include "TFG_SourceCode/Vehicle/AI/AIVehicleController.h"

RacingState::RacingState(AAIVehicleController* VehicleController): State(VehicleController)
{
	SetUp();
}

void RacingState::SetUp()
{
	State::SetUp();
	vehicle = vehicleController->GetVehicle();
}

void RacingState::Steering()
{
	FVector checkpointLocation = vehicleController->GetCurrentCheckpoint()->GetActorLocation();
	FVector vehicleLocation = vehicle->GetActorLocation();

	FVector direction = checkpointLocation - vehicleLocation;
	direction.Normalize();

	float angle = acos(FVector::DotProduct(vehicle->GetActorForwardVector(), direction)) * (180 / PI);
	if (FVector::DotProduct(vehicle->GetActorForwardVector(),
	                        FVector::CrossProduct(vehicle->GetActorForwardVector(), direction)) < 0)
	{
		angle = -angle;
	}

	DrawDebugLine(vehicle->GetWorld(), vehicleLocation, vehicleLocation + direction * 1000, FColor::Purple, false, -1,
	              0, 5);
	DrawDebugLine(vehicle->GetWorld(), vehicleLocation, vehicleLocation + vehicle->GetActorForwardVector() * 1000,
	              FColor::Blue, false, -1,
	              0, 5);
	if (angle > 10)//TODO VARIABLE
		vehicle->Turn(angle / 90.0f);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, vehicle->GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(
			                                 TEXT("Angle: %f"), angle));
	}
}

void RacingState::Update()
{
	State::Update();
	Steering();
	vehicle->Accelerate();
}

void RacingState::Exit()
{
	State::Exit();
}
