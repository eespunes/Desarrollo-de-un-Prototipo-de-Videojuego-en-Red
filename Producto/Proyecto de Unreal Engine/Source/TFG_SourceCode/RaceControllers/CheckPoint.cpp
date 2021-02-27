// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "CheckPoint.h"


#include "DrawDebugHelpers.h"
#include "TFG_SourceCode/Vehicle/VehiclePawn.h"
#include "TFG_SourceCode/Vehicle/Components/RaceComponent.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);

	leftLocation = GetActorLocation() + GetActorRightVector() * AiOffsetLeft;
	rightLocation = GetActorLocation() + GetActorRightVector() * AiOffsetRight;

	DrawDebugLine(GetWorld(), leftLocation, leftLocation + GetActorUpVector() * 1000, FColor::Purple, false,
              1000000,
              0, 15);
	DrawDebugLine(GetWorld(), rightLocation, rightLocation + GetActorUpVector() * 1000,
                  FColor::Purple, false, 1000000,
                  0, 15);
}

void ACheckPoint::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	AVehiclePawn* vehiclePawn = Cast<AVehiclePawn>(otherActor);
	if (vehiclePawn)
	{
		vehiclePawn->GetRaceComponent()->SetCheckpoint(position);
	}
}

int ACheckPoint::GetPosition() const
{
	return position;
}

FVector ACheckPoint::GetLocation()
{
	return FVector(
        FMath::RandRange(leftLocation.X, rightLocation.X),
        FMath::RandRange(leftLocation.Y, rightLocation.Y),
        FMath::RandRange(leftLocation.Z, rightLocation.Z)
    );
}
