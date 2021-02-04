#include "RacingState.h"

#include "TFG_SourceCode/Vehicle/AI/AIVehicleController.h"

RacingState::RacingState(AAIVehicleController* VehicleController): State(VehicleController)
{
}

void RacingState::SetUp()
{
	State::SetUp();
	vehicle = vehicleController->GetVehicle();
}

void RacingState::Update()
{
	State::Update();

	FVector checkpointLocation=vehicleController->GetCurrentCheckpoint()->GetActorLocation()
	vehicleController->Print(vehicleController->GetCurrentCheckpoint()->GetActorLocation().X);
}

void RacingState::Exit()
{
	State::Exit();
}
