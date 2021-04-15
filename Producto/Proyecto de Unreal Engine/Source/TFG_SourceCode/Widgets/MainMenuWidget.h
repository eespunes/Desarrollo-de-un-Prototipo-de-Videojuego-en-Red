// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "HttpModule.h"
#include "Blueprint/UserWidget.h"

#include "MainMenuWidget.generated.h"

class UHTTPCommunication;
/**
 * 
 */
UCLASS()
class TFG_SOURCECODE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	 UMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	void OnResponseLoginReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
private:
    const FString URL="https://tfg-videojocs.herokuapp.com";
	FHttpModule* http;
	bool isLoggedIn;
	bool response;
	UHTTPCommunication* controller;

	UFUNCTION(BlueprintCallable)
	void Login(FString username, FString password, bool save);
	UFUNCTION(BlueprintCallable)
	bool Logout();
	UFUNCTION(BlueprintCallable)
	void Register(FString username, FString password, FString email);
	UFUNCTION(BlueprintPure)
	bool GetResponse() const;
	UFUNCTION(BlueprintPure)
	bool GetLogin() const;
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
	FString username;
	UPROPERTY()
	FString password;
	UPROPERTY()
	FString email;
};