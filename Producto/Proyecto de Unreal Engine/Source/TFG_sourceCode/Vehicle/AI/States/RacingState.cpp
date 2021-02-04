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
	UE_LOG(LogTemp, Warning, TEXT("%f"), vehicleController->GetCurrentCheckpoint()->GetActorLocation().X);
}

void RacingState::Exit()
{
	State::Exit();
}
