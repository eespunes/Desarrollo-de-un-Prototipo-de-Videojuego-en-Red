// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "LokiTrick.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API ALokiTrick : public AObjectBase
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
	void UseObject(int32 desiredPosition);
protected:
	virtual void SetUp() override;
	virtual void Stop() override;
	virtual void SendObjectEvent() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float duration = 3;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AActor> particle;
	float timer;
	TArray<AVehiclePawn*> tricked;
	int32 position;
};

USTRUCT()
struct TFG_SOURCECODE_API FLokiStruct
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
	int32 position;
};
