// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Mjolnir.h"


#include "DrawDebugHelpers.h"
#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AMjolnir::SetUp()
{
	Super::SetUp();
}

void AMjolnir::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	Super::OnOverlapBegin(ownerActor, otherActor);
}

void AMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (used)
	{
		if (currentCheckPoint->IsPlayerInPlane(GetActorLocation()))
		{
			currentCheckPoint = gameInstance->GetLevel()->GetCheckpoint(
				(currentCheckPoint->GetPosition() + 1) % gameInstance->GetLevel()->GetNumberOfCheckpoints());
			timer++;
		}
		FVector checkpointLocation;
		if (objective)
		{
			if (objective->GetCheckpointPosition() == currentCheckPoint->GetPosition())
			{
				checkpointLocation = objective->GetOwner()->GetActorLocation() + objective->GetOwner()->
					GetActorUpVector() * distanceToFloor;
			}
			else
			{
				checkpointLocation = gameInstance->GetLevel()->GetCheckpoint(currentCheckPoint->GetPosition())->GetActorLocation() + gameInstance->GetLevel()
					->
					GetCheckpoint(currentCheckPoint->GetPosition())->GetActorUpVector() * distanceToFloor;
			}
		}
		else
		{
			checkpointLocation = gameInstance->GetLevel()->GetCheckpoint(currentCheckPoint->GetPosition())->GetActorLocation() + gameInstance->GetLevel()->
				GetCheckpoint(currentCheckPoint->GetPosition())->GetActorUpVector() * distanceToFloor;

			if (timer >= ttl)
			{
				Stop();
			}
			// timer += DeltaTime;
		}
		forward = checkpointLocation - GetActorLocation();

		forward.Normalize();

		SetActorRotation(forward.Rotation());

		SetActorLocation(GetActorLocation() + forward * speed * DeltaTime);

		rotationAnimation += initialRotationAnimation * DeltaTime;
		AddActorLocalRotation(FRotator(rotationAnimation, 0, 0));
	}
}

void AMjolnir::UseObject()
{
	Super::UseObject();

	int32 positionObjective = vehicle->GetRaceComponent()->GetPosition() - 2;
	objective = gameInstance->GetLevel()->GetVehicleByPosition(positionObjective);
	currentCheckPoint = vehicle->GetRaceComponent()->GetCurrentCheckPoint();

	SendObjectEvent();

	vehicle->RemoveObject();
}

void AMjolnir::UseObject(FString username)
{
	Super::UseObject();
	objective = gameInstance->GetLevel()->GetVehicleByUsername(username);
	currentCheckPoint = vehicle->GetRaceComponent()->GetCurrentCheckPoint();
	vehicle->RemoveObject();
}

void AMjolnir::Stop()
{
	Super::Stop();
	Destroy();
}

void AMjolnir::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FMjolnirStruct* objectData = new FMjolnirStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->objectiveUsername = objective ? objective->GetUsername() : "";
		objectData->used = used;
		FJsonObjectConverter::UStructToJsonObjectString<FMjolnirStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}
