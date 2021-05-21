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

	ACheckPoint* checkPoint = Cast<ACheckPoint>(otherActor);
	if (checkPoint)
	{
		if (expectedCheckPoint == checkPoint->GetPosition())
		{
			expectedCheckPoint = (checkPoint->GetPosition() + 1) % level->GetNumberOfCheckpoints();
		}
	}
}

void AMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (used)
	{
		FVector checkpointLocation;
		if (objective)
		{
			if (objective->GetExpectedCheckpoint() == expectedCheckPoint)
			{
				checkpointLocation = objective->GetOwner()->GetActorLocation() + objective->GetOwner()->
					GetActorUpVector() * distanceToFloor;
			}
			else
			{
				checkpointLocation = level->GetCheckpoint(expectedCheckPoint)->GetActorLocation() + level->
					GetCheckpoint(expectedCheckPoint)->GetActorUpVector() * distanceToFloor;
			}
		}
		else
		{
			checkpointLocation = level->GetCheckpoint(expectedCheckPoint)->GetActorLocation() + level->
				GetCheckpoint(expectedCheckPoint)->GetActorUpVector() * distanceToFloor;

			if (timer >= ttl)
			{
				Stop();
			}
			timer += DeltaTime;
		}
		forward = checkpointLocation - GetActorLocation();

		forward.Normalize();

		SetActorRotation(forward.Rotation());
		SetActorLocation(GetActorLocation() + forward * speed);

		rotationAnimation += initialRotationAnimation;
		AddActorLocalRotation(FQuat(FRotator(rotationAnimation, 0, 0)), true, 0, ETeleportType::None);
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
		expectedCheckPoint = vehicle->GetRaceComponent()->GetExpectedCheckpoint();
	}
}

void AMjolnir::Stop()
{
	Super::Stop();
	Destroy();
}
