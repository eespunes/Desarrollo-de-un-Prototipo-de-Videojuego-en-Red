// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"


#include "RaceController.generated.h"

class URaceComponent;
class ACheckPoint;
UCLASS()
class TFG_SOURCECODE_API ARaceController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARaceController();

protected:
	TArray<ACheckPoint*> checkpoints;
	TArray<URaceComponent*> vehicles;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void FindVehicles();
	void FindCheckpoints();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ACheckPoint* GetCheckpoint(int32 idx);
	int GetNumberOfCheckpoints() const;
	URaceComponent* GetVehicle(int32 position);
};
