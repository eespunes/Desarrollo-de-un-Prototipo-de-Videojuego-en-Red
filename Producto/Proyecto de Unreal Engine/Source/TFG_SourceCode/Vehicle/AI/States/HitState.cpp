#include "HitState.h"

#include "IdleState.h"
#include "RacingState.h"
#include "StartState.h"
#include "TFG_SourceCode/Vehicle/AI/AIVehicleController.h"

HitState::HitState(AAIVehicleController* vehicleController): State(vehicleController)
{
	HitState::SetUp();
}

void HitState::SetUp()
{
	State::SetUp();
	UE_LOG(LogTemp,Warning,TEXT("HIT"));
}

void HitState::Update()
{
	State::Update();
	if (!vehicleController->GetVehicle()->GetHasBeenHit())
		Exit();
}

void HitState::Exit()
{
	State::Exit();

	vehicleController->SetCurrentState(new IdleState(vehicleController, new RacingState(vehicleController)));
}
