// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "Engine/DirectionalLight.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"
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
	virtual void Tick(float DeltaTime) override;
private:
	float timer;
	float time = 1;
	URaceGameInstance* gameInstance;
	ADirectionalLight* directionalLight;
};
