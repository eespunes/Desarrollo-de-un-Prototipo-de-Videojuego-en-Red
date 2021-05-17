// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TFG_SourceCode/Vehicle/IVehicle.h"

#include "VehicleNetworkController.generated.h"

class AVehiclePawn;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AVehicleNetworkController : public AAIController, public IVehicle
{
	GENERATED_BODY()
public:
	virtual void Accelerate() override;
	virtual void Brake() override;
	virtual void Steer(float value) override;
	virtual void Drift() override;
	virtual void UseObject() override;
	bool IsAccelerating();
	bool IsBraking();
	bool IsDrifting();


protected:
	AVehiclePawn* vehiclePawn;
	virtual void BeginPlay() override;
};
