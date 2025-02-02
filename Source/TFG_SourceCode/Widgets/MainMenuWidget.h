// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "HttpModule.h"
#include "Blueprint/UserWidget.h"

#include "MainMenuWidget.generated.h"

class URaceGameInstance;
struct FPlayerStruct;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnResponseUserReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FPlayerStruct* playerLoggedIn;
	bool responseReceived;
	URaceGameInstance* gameInstance;
	bool responseCorrect;
	TArray<FString> playersInRace;

	UFUNCTION(BlueprintCallable)
	void BeginMultiplayer();

	UFUNCTION(BlueprintCallable)
	void Login(FString username, FString password);
	UFUNCTION(BlueprintCallable)
	void Logout();
	UFUNCTION(BlueprintCallable)
	void Register(FString username, FString password, FString email);
	UFUNCTION(BlueprintCallable)
	void FindUserData(FString username);
	UFUNCTION(BlueprintCallable)
	void DeletePlayerInfo();

	UFUNCTION(BlueprintCallable)
	void ConnectToServer();
	UFUNCTION(BlueprintPure)
	bool GetRaceConnection();
	UFUNCTION(BlueprintPure)
	bool GetRaceFound();
	UFUNCTION(BlueprintPure)
	bool GetRaceFoundSuccessfully();
	UFUNCTION(BlueprintPure)
	FString GetRaceID();
	UFUNCTION(BlueprintPure)
	FString GetTimer();
	UFUNCTION(BlueprintCallable)
	void ConnectToRace();
	UFUNCTION(BlueprintCallable)
	void DisconnectToServer();

	UFUNCTION(BlueprintPure)
	TArray<FString> GetPlayersMatchMaking();

	UFUNCTION(BlueprintPure)
	bool GetResponseReceived() const;
	UFUNCTION(BlueprintPure)
	bool GetResponseCorrect() const;

	UFUNCTION(BlueprintPure)
	FString GetUsername() const;
	UFUNCTION(BlueprintPure)
	FString GetPassword() const;
	UFUNCTION(BlueprintPure)
	FString GetEmail() const;
	UFUNCTION(BlueprintPure)
	int32 GetRegistrationDate() const;
	UFUNCTION(BlueprintPure)
	int32 GetLevel() const;
	UFUNCTION(BlueprintPure)
	int32 GetExperiencePoints() const;
	UFUNCTION(BlueprintPure)
	int32 GetGoldenSteeringWheel() const;
	UFUNCTION(BlueprintPure)
	int32 GetSilverSteeringWheel() const;
	UFUNCTION(BlueprintPure)
	int32 GetBronzeSteeringWheel() const;
	UFUNCTION(BlueprintPure)
	int32 GetWoodenSteeringWheel() const;
	UFUNCTION(BlueprintPure)
	bool PlayerIsLoggedIn() const;
};

USTRUCT()
struct TFG_SOURCECODE_API FLoginStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString username;
	UPROPERTY()
	FString password;
};

USTRUCT()
struct TFG_SOURCECODE_API FLogoutStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString username;
};

USTRUCT()
struct TFG_SOURCECODE_API FRegisterStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString username;
	UPROPERTY()
	FString password;
	UPROPERTY()
	FString email;
};

USTRUCT()
struct TFG_SOURCECODE_API FPlayerStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString pla_username;
	UPROPERTY()
	FString pla_password;
	UPROPERTY()
	FString pla_email;
	UPROPERTY()
	FString pla_registrationdate;
	UPROPERTY()
	int32 pla_experiencepoints;
	UPROPERTY()
	int32 pla_level;
	UPROPERTY()
	int32 pla_goldensteeringwheel;
	UPROPERTY()
	int32 pla_silversteeringwheel;
	UPROPERTY()
	int32 pla_bronzesteeringwheel;
	UPROPERTY()
	int32 pla_woodensteeringwheel;
	UPROPERTY()
	int32 pla_aiID;

	FPlayerStruct()
	{
	};

	FPlayerStruct(FPlayerStruct* other)
	{
		this->pla_username = other->pla_username;
		this->pla_password = other->pla_password;
		this->pla_email = other->pla_email;
		this->pla_registrationdate = other->pla_registrationdate;
		this->pla_experiencepoints = other->pla_experiencepoints;
		this->pla_level = other->pla_level;
		this->pla_goldensteeringwheel = other->pla_goldensteeringwheel;
		this->pla_silversteeringwheel = other->pla_silversteeringwheel;
		this->pla_bronzesteeringwheel = other->pla_bronzesteeringwheel;
		this->pla_woodensteeringwheel = other->pla_woodensteeringwheel;
		this->pla_aiID = other->pla_aiID;
	};
};

USTRUCT()
struct TFG_SOURCECODE_API FPlayingPlayerStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString username;

	FPlayingPlayerStruct()
	{
	};

	FPlayingPlayerStruct(FPlayingPlayerStruct* other)
	{
		this->username = other->username;
	};
};

USTRUCT()
struct TFG_SOURCECODE_API FFindRaceStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	FString username;
	UPROPERTY()
	FString levelID;
};
