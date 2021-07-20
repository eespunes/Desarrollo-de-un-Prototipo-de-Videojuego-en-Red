#pragma once

class AVehiclePawn;

class IObject
{
public:
	virtual void UseObject() =0;
	virtual void SetVehicle(AVehiclePawn* VehiclePawn) =0;
	virtual void SetType(int32 type) =0;
protected:
	virtual void SetUp() =0;
	virtual void Stop() =0;
	virtual void SendObjectEvent() =0;
};
