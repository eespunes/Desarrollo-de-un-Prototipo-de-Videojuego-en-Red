// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include <vector>


#include "TFG_SourceCode/Objects/Base/ObjectBase.h"
#include "GameFramework/Actor.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

#include "ObjectContainer.generated.h"

UCLASS()
class TFG_SOURCECODE_API AObjectContainer : public AActor
{
	GENERATED_BODY()

	bool enabled;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess="true"))
	float timeToEnable;
	float timer;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* mesh;

	URaceGameInstance* gameInstance;
	std::vector<std::vector<float>> objectProbabilities;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int32 GetObject();
	TSubclassOf<UObject> SpawnObject(int32 position);
	void DisableContainer();

public:
	// Sets default values for this actor's properties
	AObjectContainer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(AActor* ownerActor, AActor* otherActor);
};
