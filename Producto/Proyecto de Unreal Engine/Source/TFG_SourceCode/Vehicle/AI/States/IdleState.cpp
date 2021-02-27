#include "IdleState.h"

#include "StartState.h"
#include "TFG_SourceCode/Vehicle/AI/AIVehicleController.h"

IdleState::IdleState(AAIVehicleController* vehicleController, State* state): State(vehicleController)
{
	IdleState::SetUp();
	currentState = state;
}

void IdleState::SetUp()
{
	State::SetUp();
	// UE_LOG(LogTemp,Warning,TEXT("IDLE"));
}

void IdleState::Update()
{
	State::Update();
	Exit();
}

void IdleState::Exit()
{
	State::Exit();
	if (currentState)
	{
		vehicleController->SetCurrentState(currentState);
	}
	else
	{
		vehicleController->SetCurrentState(new StartState(vehicleController));
	}
}
