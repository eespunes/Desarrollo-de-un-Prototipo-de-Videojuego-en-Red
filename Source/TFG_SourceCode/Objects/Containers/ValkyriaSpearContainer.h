// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"


#include "TFG_SourceCode/Objects/Simple Objects/FenrirRoar.h"
#include "TFG_SourceCode/Objects/Simple Objects/ValkyriaSpear.h"

#include "ValkyriaSpearContainer.generated.h"

/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API AValkyriaSpearContainer : public AObjectBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
	void UseObject(FVector vehiclePosition, FRotator vehicleRotation, FVector vehicleDirection);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* spawnPoint1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* spawnPoint2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* spawnPoint3;

	TArray<AValkyriaSpear*> spears;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AValkyriaSpear> objectToSpawn;
	UPROPERTY(EditAnywhere)
	float rotation;
	UPROPERTY(EditAnywhere)
	int numberOfSpears;

	int nextObject;

	virtual void SetUp() override;
	virtual void Stop() override;
	virtual void SendObjectEvent() override;
	void AddSpear(FRotator toRotate);
};

USTRUCT()
struct TFG_SOURCECODE_API FValkyriaStruct
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
	float positionX;
	UPROPERTY()
	float positionY;
	UPROPERTY()
	float positionZ;

	UPROPERTY()
	float rotationRoll;
	UPROPERTY()
	float rotationPitch;
	UPROPERTY()
	float rotationYaw;

	UPROPERTY()
	float directionX;
	UPROPERTY()
	float directionY;
	UPROPERTY()
	float directionZ;
};
