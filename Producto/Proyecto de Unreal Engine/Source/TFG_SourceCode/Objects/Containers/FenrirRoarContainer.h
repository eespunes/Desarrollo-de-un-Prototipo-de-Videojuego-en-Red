// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "TFG_SourceCode/Objects/Base/ObjectBase.h"

#include "FenrirRoarContainer.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AFenrirRoarContainer : public AObjectBase
{
	GENERATED_BODY()
public:
	virtual void UseObject() override;
protected:
	virtual void Stop() override;
	TSubclassOf<AObjectBase> GetObject();
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AObjectBase> fenrirRoar;

};
