// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "Mjolnir.h"


#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/Levels/RacingLevel.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

void AMjolnir::SetUp()
{
	Super::SetUp();
	level = Cast<ARacingLevel>(UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));
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
			currentCheckPoint = level->GetCheckpoint(
				(currentCheckPoint->GetPosition() + 1) % level->GetNumberOfCheckpoints());
		}
		FVector checkpointLocation;
		if (objective)
		{
			if (objective->GetExpectedCheckpoint() == currentCheckPoint->GetPosition())
			{
				checkpointLocation = objective->GetOwner()->GetActorLocation() + objective->GetOwner()->
					GetActorUpVector() * distanceToFloor;
			}
			else
			{
				checkpointLocation = level->GetCheckpoint(currentCheckPoint->GetPosition())->GetActorLocation() + level
					->
					GetCheckpoint(currentCheckPoint->GetPosition())->GetActorUpVector() * distanceToFloor;
			}
		}
		else
		{
			checkpointLocation = level->GetCheckpoint(currentCheckPoint->GetPosition())->GetActorLocation() + level->
				GetCheckpoint(currentCheckPoint->GetPosition())->GetActorUpVector() * distanceToFloor;

			if (timer >= ttl)
			{
				Stop();
			}
			timer += DeltaTime;
		}
		forward = checkpointLocation - GetActorLocation();

		forward.Normalize();

		SetActorRotation(forward.Rotation());
		
		SetActorLocation(GetActorLocation() + forward * speed * DeltaTime);

		rotationAnimation += initialRotationAnimation * DeltaTime;
		AddActorLocalRotation(FRotator(initialRotationAnimation * DeltaTime, 0, 0));
	}
}

void AMjolnir::UseObject()
{
	Super::UseObject();
	int32 positionObjective = vehicle->GetRaceComponent()->GetPosition() - 2;
	if (level)
	{
		objective = level->GetVehicle(positionObjective);
		if (!objective)
		{
			ttl = 10;
		}
		currentCheckPoint = vehicle->GetRaceComponent()->GetCurrentCheckPoint();
	}
	vehicle->RemoveObject();
}

void AMjolnir::Stop()
{
	Super::Stop();
	Destroy();
}
