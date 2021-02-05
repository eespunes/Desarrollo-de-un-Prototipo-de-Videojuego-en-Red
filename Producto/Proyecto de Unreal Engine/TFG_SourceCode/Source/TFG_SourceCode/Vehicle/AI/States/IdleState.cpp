#include "IdleState.h"

IdleState::IdleState(AAIVehicleController* vehicleController): State(vehicleController)
{
}

void IdleState::SetUp()
{
	
}

void IdleState::Update()
{
	Exit();
}

void IdleState::Exit()
{
}
