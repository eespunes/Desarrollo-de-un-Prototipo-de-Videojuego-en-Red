// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "TFG_SourceCode/Objects/Base/ProtectObjectBase.h"

#include "RuneShield.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ARuneShield : public AProtectObjectBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
protected:
	virtual void SetUp() override;
	virtual void Stop() override;
};
