#pragma once
#include "State.h"

class IdleState : public State
{
public:
	IdleState(AAIVehicleController* vehicleController, State* state);
	virtual void SetUp() override;
	virtual void Update() override;
	virtual void Exit() override;
protected:
	State* currentState;
};
