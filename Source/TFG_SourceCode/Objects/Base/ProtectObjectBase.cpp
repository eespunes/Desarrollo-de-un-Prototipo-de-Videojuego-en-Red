// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ProtectObjectBase.h"

#include "TFG_SourceCode/Objects/Base/HarmObjectBase.h"

void AProtectObjectBase::SetUp()
{
	Super::SetUp();
	OnActorBeginOverlap.AddDynamic(this, &AProtectObjectBase::OnOverlapBegin);
}

void AProtectObjectBase::OnOverlapBegin(AActor* ownerActor, AActor* otherActor)
{
	AHarmObjectBase* object = Cast<AHarmObjectBase>(otherActor);
	if (object)
	{
		object->Destroy();

		UseObject();
	}
}
