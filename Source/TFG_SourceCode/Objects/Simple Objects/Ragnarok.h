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
protected:
	virtual void SetUp() override;
	virtual void SendObjectEvent() override;
private:
	float timer;
	float time = 1;
	URaceGameInstance* gameInstance;
};

USTRUCT()
struct TFG_SOURCECODE_API FRagnarokStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString race;
	UPROPERTY()
	int32 id;
	UPROPERTY()
	int32 objectType;
	UPROPERTY()
	FString parentUsername;
	UPROPERTY()
	bool used;

	UPROPERTY()
	bool ragnarok;
};
