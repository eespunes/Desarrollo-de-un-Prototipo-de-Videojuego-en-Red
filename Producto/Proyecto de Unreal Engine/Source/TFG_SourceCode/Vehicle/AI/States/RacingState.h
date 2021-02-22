#pragma once

#include "State.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

class RacingState : public State
{
public:

	RacingState(AAIVehicleController* VehicleController);
	virtual void SetUp() override;
	void Steering();
	virtual void Update() override;
	virtual void Exit() override;

private:
	AVehiclePawn* vehicle;
};
