#pragma once
#include "IState.h"

class AAIVehicleController;

class State : public IState
{
protected:
	AAIVehicleController* vehicleController;
	State(AAIVehicleController* VehicleController);

public:
	virtual void SetUp() override;
	virtual void Update() override;
	virtual void Exit() override;
};
