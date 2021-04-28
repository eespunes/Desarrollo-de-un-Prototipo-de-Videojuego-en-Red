// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "RaceGameInstance.h"

float URaceGameInstance::GetDifficulty() const
{
	if (currentDifficulty == 0)
		return lowerDifficulty;
	if (currentDifficulty == 1)
		return mediumDifficulty;
	
	return higherDifficulty;
}

void URaceGameInstance::SetCurrentDifficulty(int CurrentDifficulty)
{
	currentDifficulty = CurrentDifficulty;
}
