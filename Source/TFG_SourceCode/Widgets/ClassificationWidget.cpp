// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "ClassificationWidget.h"

#include "JsonObjectConverter.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

TArray<URaceComponent*> UClassificationWidget::GetClassification()
{
	if (!gameInstance)
	{
		gameInstance = Cast<URaceGameInstance>(GetGameInstance());
	}

	return gameInstance->GetClassification();
}

void UClassificationWidget::GetRewards()
{
	//SEND POST
	//DISCONNECT SOCKET
	FString uri = "api/";

	FString json;
	FRewardStruct rewardStruct;
	rewardStruct.username = gameInstance->GetPlayer().pla_username;
	rewardStruct.value = 1;
	FJsonObjectConverter::UStructToJsonObjectString(FRewardStruct::StaticStruct(), &rewardStruct, json, 0, 0);

	for (int i = 0; i < gameInstance->GetClassification().Num(); i++)
	{
		if (gameInstance->GetClassification()[i]->GetUsername().Equals(rewardStruct.username))
		{
			uri += i == 0 ? "golden" : i == 1 ? "silver" : i == 2 ? "bronze" : "wood";
		}
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	gameInstance->SendPost(Request, uri, json);
	gameInstance->DisconnectSocket();
}
