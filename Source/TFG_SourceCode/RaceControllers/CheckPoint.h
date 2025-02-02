// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerBox.h"
#include "CheckPoint.generated.h"

UCLASS()
class TFG_SOURCECODE_API ACheckPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckPoint();
	int GetPosition() const;
	FVector GetLocation();
	bool IsPlayerInPlane(FVector playerLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	int position = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	float AiOffsetRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	float AiOffsetLeft;
	FVector leftLocation;
	FVector rightLocation;
	FPlane plane;
};
