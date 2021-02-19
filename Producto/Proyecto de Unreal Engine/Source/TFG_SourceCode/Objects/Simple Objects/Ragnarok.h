// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "Ragnarok.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ARagnarok : public AObjectBase
{
	GENERATED_BODY()

public:
	virtual void UseObject() override;
};
