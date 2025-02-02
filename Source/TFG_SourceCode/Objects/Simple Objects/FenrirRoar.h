// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"

#include "FenrirRoar.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AFenrirRoar : public AObjectBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
	virtual void Stop() override;
protected:
	virtual void SetUp() override;
	UFUNCTION()
	virtual void OnOverlapBegin(AActor* ownerActor, AActor* otherActor);
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float boostDuration = 10;
	float timer;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float boostIncrease = 0.75f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AObjectBase> fenrirRoar;
};
