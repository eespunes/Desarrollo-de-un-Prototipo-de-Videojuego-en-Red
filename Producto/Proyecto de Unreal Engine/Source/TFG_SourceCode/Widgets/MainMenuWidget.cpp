// Copyright © Erik Espuñes Juberó 2021 All Rights Reserved


#include "MainMenuWidget.h"

#include <string>


#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "TFG_SourceCode/Controllers/HTTPCommunication.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//When the object is constructed, Get the HTTP module
	http = &FHttpModule::Get();
	controller = NewObject<UHTTPCommunication>();
}

bool UMainMenuWidget::GetResponse() const
{
	return response;
}

bool UMainMenuWidget::GetLogin() const
{
	return isLoggedIn;
}

void UMainMenuWidget::OnResponseLoginReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	response = true;
	if (controller->IsSuccessful(Response->GetResponseCode()))
	{
		isLoggedIn = true;
	}
}

void UMainMenuWidget::Login(FString username, FString password, bool save)
{
	response = false;
	FString json;
	FLoginStruct loginStruct;
	loginStruct.username = username;
	loginStruct.password = password;
	FJsonObjectConverter::UStructToJsonObjectString(FLoginStruct::StaticStruct(), &loginStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseLoginReceived);

	controller->SendPost(Request, "/api/login", json);
}

bool UMainMenuWidget::Logout()
{
	return false;
}

void UMainMenuWidget::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (controller->IsSuccessful(Response->GetResponseCode()))
	{
		response = true;
	}
}
void UMainMenuWidget::Register(FString username, FString password, FString email)
{
	response = false;
	FString json;
	FRegisterStruct registerStruct;
	registerStruct.username = username;
	registerStruct.password = password;
	registerStruct.email=email;
	FJsonObjectConverter::UStructToJsonObjectString(FRegisterStruct::StaticStruct(), &registerStruct, json, 0, 0);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UMainMenuWidget::OnResponseReceived);

	controller->SendPost(Request, "/api/player", json);
}
