// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "TFG_SourceCode/Objects/Simple Objects/RuneShield.h"

void ARuneShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARuneShield::UseObject()
{
	Super::UseObject();
	SendObjectEvent();
	Stop();
}

void ARuneShield::SetUp()
{
	objectType=1;
	Super::SetUp();
}

void ARuneShield::Stop()
{
	Super::Stop();
	Destroy();
}
