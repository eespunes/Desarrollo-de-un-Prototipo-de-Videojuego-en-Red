// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"


#include "TFG_SourceCode/Objects/IObject.h"
#include "GameFramework/Actor.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

#include "ObjectBase.generated.h"

class IObject;
class AVehiclePawn;

UCLASS()
class TFG_SOURCECODE_API AObjectBase : public AActor, public IObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObjectBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UseObject() override;
	virtual void SetVehicle(AVehiclePawn* VehiclePawn) override;
	virtual void SetType(int32 type) override;
	bool IsUsed();
	bool IsSender();
	
	void SetID(int32 ID);
	FString GetUsername();
	int32 GetID();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* mesh;
	URaceGameInstance* gameInstance;

protected:
	bool used;
	AVehiclePawn* vehicle;
	int32 id = -1;
	int32 objectType;
	bool isSender;
	FString parentUsername;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetUp() override;
	virtual void Stop() override;
	virtual void SendObjectEvent() override;
};

USTRUCT()
struct TFG_SOURCECODE_API FObjectStruct
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
};
