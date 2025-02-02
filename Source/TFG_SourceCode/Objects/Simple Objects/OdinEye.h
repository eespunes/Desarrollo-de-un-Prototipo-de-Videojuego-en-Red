// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "OdinEye.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AOdinEye : public AObjectBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	void Teleport();
	virtual void UseObject() override;
	void UseObject(int checkpointPosition);
protected:
	virtual void SetUp() override;
	virtual void Stop() override;
	virtual void SendObjectEvent() override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class AActor> particle;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float min;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	float max;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Boost",meta=(AllowPrivateAccess="true"))
	FVector offset;
	ACheckPoint* checkPoint;
};

USTRUCT()
struct TFG_SOURCECODE_API FOdinStruct
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
	int32 destination;
};
