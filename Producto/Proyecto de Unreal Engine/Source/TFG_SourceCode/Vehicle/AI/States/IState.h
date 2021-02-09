#pragma once

class IState
{
public:
	virtual void SetUp() =0;
	virtual void Update() =0;
	virtual void Exit() =0;
};
