// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "IdunApple.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AIdunApple : public AObjectBase
{
	GENERATED_BODY()
public:
	AIdunApple();

	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
protected:
	virtual void SetUp() override;
	virtual void Stop() override;

	UFUNCTION()
	virtual void OnOverlapBegin(AActor* ownerActor, AActor* otherActor);

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true")

	)
	UStaticMeshComponent* shield;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float boostDuration = 3;
	float timer;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float boostIncrease = 1.5f;
};
