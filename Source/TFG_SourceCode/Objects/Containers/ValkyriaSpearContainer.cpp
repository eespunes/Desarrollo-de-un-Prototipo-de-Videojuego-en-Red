// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ValkyriaSpearContainer.h"

#include "JsonObjectConverter.h"

void AValkyriaSpearContainer::Tick(float DeltaTime)
{
}

void AValkyriaSpearContainer::UseObject()
{
	Super::UseObject();
	spears[nextObject]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	spears[nextObject]->SetOwner(nullptr);
	spears[nextObject]->SetVehicle(vehicle);
	spears[nextObject]->UseObject();

	SendObjectEvent();

	nextObject++;

	if (nextObject == numberOfSpears)
	{
		Stop();
	}
	else
	{
		used = false;
	}
}

void AValkyriaSpearContainer::UseObject(FVector vehiclePosition, FRotator vehicleRotation, FVector vehicleDirection)
{
	spears[nextObject]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	spears[nextObject]->SetOwner(nullptr);
	spears[nextObject]->SetVehicle(vehicle);
	spears[nextObject]->UseObject(vehiclePosition, vehicleRotation, vehicleDirection);

	nextObject++;

	if (nextObject == numberOfSpears)
	{
		Stop();
	}
}

void AValkyriaSpearContainer::SetUp()
{
	objectType = 3;
	Super::SetUp();
	FRotator initalRotation = mesh->GetComponentRotation();
	initalRotation.Add(0, -rotation, 0);
	float angleToRotate = (rotation * 2) / numberOfSpears;
	for (int i = 0; i < numberOfSpears; i++)
	{
		AddSpear(initalRotation.Add(0, angleToRotate, 0));
	}
	nextObject = 0;
}

void AValkyriaSpearContainer::AddSpear(FRotator toRotate)
{
	AValkyriaSpear* valkyriaSpear = GetWorld()->SpawnActor<AValkyriaSpear>(
		objectToSpawn, mesh->GetComponentLocation(), toRotate);
	valkyriaSpear->SetOwner(this);
	valkyriaSpear->AttachToComponent(mesh, FAttachmentTransformRules::KeepWorldTransform);
	valkyriaSpear->SetVehicle(vehicle);
	spears.Add(valkyriaSpear);
}

void AValkyriaSpearContainer::Stop()
{
	Super::Stop();
}

void AValkyriaSpearContainer::SendObjectEvent()
{
	if (isSender && used)
	{
		FString json;
		FValkyriaStruct* objectData = new FValkyriaStruct();

		objectData->race = gameInstance->GetRaceID();
		objectData->id = id;
		objectData->objectType = objectType;
		objectData->parentUsername = parentUsername;
		objectData->used = true;

		objectData->positionX = vehicle->GetActorLocation().X;
		objectData->positionY = vehicle->GetActorLocation().Y;
		objectData->positionZ = vehicle->GetActorLocation().Z;

		objectData->directionX = vehicle->GetForward().X;
		objectData->directionY = vehicle->GetForward().Y;
		objectData->directionZ = vehicle->GetForward().Z;

		objectData->rotationRoll = vehicle->GetActorRotation().Roll;
		objectData->rotationPitch = vehicle->GetActorRotation().Pitch;
		objectData->rotationYaw = vehicle->GetActorRotation().Yaw;
		FJsonObjectConverter::UStructToJsonObjectString<FValkyriaStruct>(*objectData, json, 0, 0);

		gameInstance->SendObjectEvents(json);
	}
	else
	{
		Super::SendObjectEvent();
	}
}
