#pragma once

class AVehiclePawn;

class IObject
{
public:
	virtual void UseObject() =0;
	virtual void SetVehicle(AVehiclePawn* VehiclePawn) =0;
protected:
	virtual void SetUp() =0;
	virtual void Stop() =0;
};
