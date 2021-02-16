// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TFG_SOURCECODE_API IVehicle
{
public:
	virtual void Accelerate() =0;
	virtual void Brake() =0;
	virtual  void Turn(float value)=0;
	virtual  void Drift()=0;
	virtual  void UseObject()=0;
};
