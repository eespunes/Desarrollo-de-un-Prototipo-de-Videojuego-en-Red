// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "TFG_SourceCode/Objects/Base/HarmObjectBase.h"

#include "ValkyriaSpear.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AValkyriaSpear : public AHarmObjectBase
{
	GENERATED_BODY()
public:

	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
	void UseObject(FVector position, FRotator rotation, FVector direction);
protected:
	virtual void SetUp() override;
	virtual void Stop() override;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement",meta=(AllowPrivateAccess="true"))
	float speed = 50;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement",meta=(AllowPrivateAccess="true"))
	float ttl = 10;
	FVector forward;
	float timer;
};
