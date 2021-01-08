// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLevelActor.h"

#include "Kismet/GameplayStatics.h"

void ABaseLevelActor::BeginPlay()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), foundActors);

	int c = 0;
	for (AActor* actor : foundActors)
	{
		ACheckPoint* checkpoint = Cast<ACheckPoint>(actor);
		if (checkpoint)
		{
			UE_LOG(LogTemp, Warning, TEXT("CheckPoint #%i -> %f, %f, %f"), c++, checkpoint->GetActorLocation().X,checkpoint->GetActorLocation().Y,checkpoint->GetActorLocation().Z);
			checkpoints.Add(checkpoint);
		}
	}
}
