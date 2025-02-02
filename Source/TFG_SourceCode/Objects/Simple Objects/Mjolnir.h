// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "TFG_SourceCode/Objects/Base/HarmObjectBase.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"


#include "Mjolnir.generated.h"

class URaceComponent;
class ARacingLevel;

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AMjolnir : public AHarmObjectBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
	void UseObject(FString username);
	virtual void Stop() override;
protected:
	virtual void SetUp() override;
	virtual void OnOverlapBegin(AActor* ownerActor, AActor* otherActor) override;
	void Movement();
	virtual void SendObjectEvent() override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USceneComponent* root;
	URaceComponent* objective;
	ACheckPoint* currentCheckPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float ttl = 10;
	float timer;
	FVector forward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float speed = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	int distanceToFloor = 100;
	float rotationAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float initialRotationAnimation = 5;
};

USTRUCT()
struct TFG_SOURCECODE_API FMjolnirStruct
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
	FString objectiveUsername;
};
