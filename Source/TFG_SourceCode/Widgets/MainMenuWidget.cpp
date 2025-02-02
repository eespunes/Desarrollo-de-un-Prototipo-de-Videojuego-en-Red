// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "MainMenuWidget.h"

#include <string>


#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "TFG_SourceCode/GameModes/RaceGameInstance.h"

bool UMainMenuWidget::GetResponseReceived() const
{
	return responseReceived;
}

bool UMainMenuWidget::GetResponseCorrect() const
{
	return responseCorrect;
}

void UMainMenuWidget::BeginMultiplayer()
{
	if (!gameInstance)
	{
		gameInstance = Cast<URaceGameInstance>(GetGameInstance());
	}
}

void UMainMenuWidget::Login(FString username, FString password)
{
	responseCorrect = false;
	responseReceived = false;

	FString json;
	FLoginStruct loginStruct;
	loginStruct.username = username;
	loginStruct.password = password;
	FJsonObjectConverter::UStructToJsonObjectString(FLoginStruct::StaticStruct(), &loginStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	gameInstance->SendPost(Request, "api/login", json);
}

void UMainMenuWidget::Logout()
{
	responseCorrect = false;
	responseReceived = false;

	FString json;
	FLogoutStruct logoutStruct;
	logoutStruct.username = GetUsername();
	FJsonObjectConverter::UStructToJsonObjectString(FLogoutStruct::StaticStruct(), &logoutStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	gameInstance->SendPost(Request, "api/logout", json);
}

void UMainMenuWidget::DeletePlayerInfo()
{
	playerLoggedIn = nullptr;
}

TArray<FString> UMainMenuWidget::GetPlayersMatchMaking()
{
	if (gameInstance->GetPlayersUpdated())
		playersInRace = gameInstance->GetPlayersInRace();
	return playersInRace;
}

void UMainMenuWidget::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (gameInstance->IsSuccessful(Response->GetResponseCode()))
	{
		responseCorrect = true;
	}

	responseReceived = true;
}

void UMainMenuWidget::Register(FString username, FString password, FString email)
{
	responseCorrect = false;
	responseReceived = false;
	
	FString json;
	FRegisterStruct registerStruct;
	registerStruct.username = username;
	registerStruct.password = password;
	registerStruct.email = email;
	FJsonObjectConverter::UStructToJsonObjectString(FRegisterStruct::StaticStruct(), &registerStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	gameInstance->SendPost(Request, "api/player", json);
}

void UMainMenuWidget::OnResponseUserReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (gameInstance->IsSuccessful(Response->GetResponseCode()))
	{
		responseCorrect = true;
		TArray<FPlayerStruct> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(Response->GetContentAsString(), &data, 0, 0);

		if (data.Num() > 0)
		{
			playerLoggedIn = new FPlayerStruct(&data[0]);
			gameInstance->SetPlayer(playerLoggedIn);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Response Code: %i"), Response->GetResponseCode())
	}
	responseReceived = true;
}

void UMainMenuWidget::FindUserData(FString username)
{
	responseCorrect = false;
	responseReceived = false;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseUserReceived);
	gameInstance->SendGet(Request, "api/player/" + username);
}

void UMainMenuWidget::ConnectToServer()
{
	gameInstance->DisconnectSocket();
	gameInstance->ConnectSocket();
}

bool UMainMenuWidget::GetRaceConnection()
{
	return gameInstance->SocketIsConnected();
}

bool UMainMenuWidget::GetRaceFound()
{
	return gameInstance->GetRaceFound();
}

bool UMainMenuWidget::GetRaceFoundSuccessfully()
{
	return gameInstance->GetRaceFoundSuccessfully();
}

FString UMainMenuWidget::GetRaceID()
{
	return gameInstance->GetRaceID();
}

FString UMainMenuWidget::GetTimer()
{
	return gameInstance->Timer();
}

void UMainMenuWidget::ConnectToRace()
{
	responseCorrect = false;
	responseReceived = false;
	gameInstance->ReceiveFindRaceEvents();

	FString json;
	FFindRaceStruct findRaceStruct;
	findRaceStruct.username = playerLoggedIn->pla_username;
	findRaceStruct.levelID = TEXT("L001");
	FJsonObjectConverter::UStructToJsonObjectString(FFindRaceStruct::StaticStruct(), &findRaceStruct, json, 0, 0);
	gameInstance->SendEvents(TEXT("FindRace"), json);
}


void UMainMenuWidget::DisconnectToServer()
{
	gameInstance->DisconnectRace();
}

FString UMainMenuWidget::GetUsername() const
{
	if (!playerLoggedIn)
		return "";
	return playerLoggedIn->pla_username;
}

FString UMainMenuWidget::GetPassword() const
{
	if (!playerLoggedIn)
		return "";
	return playerLoggedIn->pla_password;
}

FString UMainMenuWidget::GetEmail() const
{
	if (!playerLoggedIn)
		return "";
	return playerLoggedIn->pla_email;
}

int32 UMainMenuWidget::GetRegistrationDate() const
{
	if (!playerLoggedIn)
		return -1;
	// return playerLoggedIn->registrationDate;
	return 2021;
}

int32 UMainMenuWidget::GetLevel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_level;
}

int32 UMainMenuWidget::GetExperiencePoints() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_experiencepoints;
}

int32 UMainMenuWidget::GetGoldenSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_goldensteeringwheel;
}

int32 UMainMenuWidget::GetSilverSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_silversteeringwheel;
}

int32 UMainMenuWidget::GetBronzeSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_bronzesteeringwheel;
}

int32 UMainMenuWidget::GetWoodenSteeringWheel() const
{
	if (!playerLoggedIn)
		return -1;
	return playerLoggedIn->pla_woodensteeringwheel;
}

bool UMainMenuWidget::PlayerIsLoggedIn() const
{
	if (!playerLoggedIn)
		return false;
	return true;
}
