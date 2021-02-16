// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "OdinEye.h"

#include <algorithm>


#include "Kismet/GameplayStatics.h"
#include "TFG_SourceCode/RaceControllers/CheckPoint.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"

void AOdinEye::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOdinEye::UseObject()
{
	Super::UseObject();

	ARacingLevel* level = Cast<ARacingLevel>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ARacingLevel::StaticClass()));

	FVector position;
	FRotator rotation;


	int32 currentPosition = vehicle->GetRaceComponent()->GetPosition() - 2;
	if (currentPosition < 0)
	{
		position = vehicle->GetRaceComponent()->GetCurrentCheckPoint()->GetActorLocation();
		rotation = vehicle->GetRaceComponent()->GetCurrentCheckPoint()->GetActorRotation();
	}
	else
	{
		int32 expectedCheckpoint = level->GetVehicle(currentPosition)->
		                                  GetExpectedCheckpoint();
		int32 leaderExpectedCheckpoint = level->GetVehicle(0)->GetExpectedCheckpoint();

		if (expectedCheckpoint == leaderExpectedCheckpoint)
		{
			expectedCheckpoint = vehicle->GetRaceComponent()->GetExpectedCheckpoint();
		}

		int32 difference = leaderExpectedCheckpoint - expectedCheckpoint;
		difference *= FMath::RandRange(min, max);
		difference = difference == 0 ? 1 : difference;
		expectedCheckpoint += difference;

		position = level->GetCheckpoint(expectedCheckpoint)->GetActorLocation();
		rotation = level->GetCheckpoint(expectedCheckpoint)->GetActorRotation();
	}


	vehicle->InstantiateParticle(particle);

	vehicle->SetActorLocation(position+offset, false, nullptr, ETeleportType::TeleportPhysics);
	vehicle->SetActorRotation(rotation, ETeleportType::TeleportPhysics);

	Stop();
	Destroy();
}

void AOdinEye::SetUp()
{
	Super::SetUp();
}

void AOdinEye::Stop()
{
	Super::Stop();
	Destroy();
}
