#pragma once
#include "State.h"

class HitState : public State
{
public:
	HitState(AAIVehicleController* vehicleController);
	virtual void SetUp() override;
	virtual void Update() override;
	virtual void Exit() override;
};
