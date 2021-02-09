#pragma once
#include "IState.h"
#include "State.h"

class StartState:public State
{
public:

	StartState(AAIVehicleController* VehicleController);

	virtual void SetUp() override;
	virtual void Update() override;
	virtual void Exit() override;
};
