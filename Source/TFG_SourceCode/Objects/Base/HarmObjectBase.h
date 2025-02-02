// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "HarmObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AHarmObjectBase : public AObjectBase
{
	GENERATED_BODY()

protected:
	virtual void SetUp() override;
	UFUNCTION()
	virtual void OnOverlapBegin(AActor* ownerActor, AActor* otherActor);
};
