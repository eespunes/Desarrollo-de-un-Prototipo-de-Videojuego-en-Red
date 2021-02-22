#include "StartState.h"


#include "RacingState.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/AI/AIVehicleController.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

StartState::StartState(AAIVehicleController* VehicleController): State(VehicleController)
{
	StartState::SetUp();
}

void StartState::SetUp()
{
	State::SetUp();
}

void StartState::Update()
{
	State::Update();
	if (vehicleController->GetVehicle()->GetRaceComponent()->CanRace())
	{
		Exit();
	}
}

void StartState::Exit()
{
	State::Exit();
	vehicleController->SetCurrentState(new RacingState(vehicleController));
}
