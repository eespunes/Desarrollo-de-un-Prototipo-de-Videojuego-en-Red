#pragma once
#include "IState.h"
#include "State.h"
#include "TFG_SourceCode/Vehicle/IVehicle.h"

class RacingState : public State
{
public:

	RacingState(AAIVehicleController* VehicleController);
	virtual void SetUp() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	IVehicle* vehicle;
};
