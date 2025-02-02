// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "Sleipnir.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ASleipnir : public AObjectBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
protected:
	virtual void SetUp() override;
	virtual void Stop() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float boostDuration = 3;
	float timer;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float boostIncrease = 1.5f;
};
