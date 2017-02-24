// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "AWebAPI.h"
#include "NonUnreal/MichelangeloAPI/URLConstants.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/UGameDataSingleton.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/CameraParameters.h"

using namespace Common;
using namespace MichelangeloAPI;

AAWebAPI::AAWebAPI()
{
	PrimaryActorTick.bCanEverTick = false;
}

FStatus AAWebAPI::Authenticate(const FString& email, const FString& password, bool rememberMe)
{	
	if (email.IsEmpty())
		return FStatus(false, TEXT("Email field must not be empty!"));

	if (password.IsEmpty())
		return FStatus(false, TEXT("Password field must not be empty!"));

	if(rememberMe)
	{
		// Save email for future use:
		auto gameDataSingleton = UGameDataSingleton::Get();
		gameDataSingleton->SetSavedEmail(email);
	}

	auto error = m_webAPI.Authenticate(Helpers::FStringToString(email), Helpers::FStringToString(password), false);

	switch(error)
	{
	case WebAPI::LoginError::None: 
		return FStatus(true, TEXT("Success!"));
	
		case WebAPI::LoginError::WrongCredentials:
		return FStatus(false, TEXT("Wrong credentials!"));
	
		default: 
		return FStatus(false, "Unexpected error.");
	}
}

void AAWebAPI::LogOut()
{
	m_webAPI.LogOut();
}

TArray<FGrammarData> AAWebAPI::GetGrammars(const FString& url) const
{
	if (!IsAuthenticated())
		return TArray<FGrammarData>();

	auto grammarsData = m_webAPI.GetGrammars(Helpers::FStringToString(url));

	TArray<FGrammarData> output;
	for (auto& grammarData : grammarsData)
	{
		FGrammarData fGrammarData;
		fGrammarData.ID = Helpers::StringToFString(grammarData.ID);
		fGrammarData.Name = Helpers::StringToFString(grammarData.Name);
		fGrammarData.Type = Helpers::StringToFString(grammarData.Type);
		output.Add(fGrammarData);
	}

	return output;
}
TArray<FGrammarData> AAWebAPI::GetGrammarsByType(EGrammarType grammarType) const
{
	TArray<FGrammarData> output;

	switch(grammarType)
	{
	case EGrammarType::Own:
		output = GetGrammars(Helpers::StringToFString(URLConstants::OwnGrammarAPI));
		break;

	case EGrammarType::Shared:
		output = GetGrammars(Helpers::StringToFString(URLConstants::SharedGrammarAPI));
		break;

	case EGrammarType::Tutorial:
		output = GetGrammars(Helpers::StringToFString(URLConstants::TutorialAPI));
		break;
	
	default: 
		break;
	}

	return output;
}

FGrammarSpecificData AAWebAPI::GetGrammarSpecificData(const FString& url, const FString& id) const
{
	if (!IsAuthenticated())
		return FGrammarSpecificData();

	return FGrammarSpecificData::FromGrammarSpecificData(m_webAPI.GetGrammarSpecificData(Helpers::FStringToString(url), Helpers::FStringToString(id)));
}

FGrammarSpecificData AAWebAPI::GetGrammarSpecificDataByType(EGrammarType grammarType, const FString& id) const
{
	FGrammarSpecificData output;

	switch (grammarType)
	{
	case EGrammarType::Own:
	case EGrammarType::Shared: // TODO check this with martin
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::OwnGrammarAPI), id);
		break;

	//case EGrammarType::Shared:
	//	output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::SharedGrammarAPI), id);
	//	break;

	case EGrammarType::Tutorial:
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::TutorialAPI), id);
		break;

	default:
		break;
	}

	return output;
}

bool AAWebAPI::GenerateGeometry(const FString& url, const FGrammarSpecificData& data, FString& errorMessage)
{
	GrammarSpecificData apiData;
	{
		apiData.ID = Helpers::FStringToString(data.ID);
		apiData.Name = Helpers::FStringToString(data.Name);
		apiData.Type = Helpers::FStringToString(data.Type);
		apiData.Code = Helpers::FStringToWString(data.Code);
		apiData.Shared = data.Shared;
		apiData.IsOwner = data.IsOwner;
	}

	CameraParameters cameraParameters;
	{
		auto* cameraManager = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;

		auto position = Helpers::TransformPositionFromUnrealToMichelangeloSpace(cameraManager->GetCameraLocation());
		cameraParameters.SetCameraPosition({ position.X, position.Y, position.Z });
		
		auto up = Helpers::TransformVectorFromUnrealToMichelangeloSpace(cameraManager->GetActorUpVector());
		cameraParameters.SetCameraUpDirection({ up.X, up.Y, up.Z });

		auto forward = Helpers::TransformVectorFromUnrealToMichelangeloSpace(cameraManager->GetActorForwardVector());
		cameraParameters.SetCameraForwardDirection({ forward.X, forward.Y, forward.Z });

		cameraParameters.SetCameraFov(cameraManager->GetFOVAngle());
		
		auto viewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		cameraParameters.SetWidth(viewportSize.X);
		cameraParameters.SetHeight(viewportSize.Y);
	}

	SceneGeometry sceneGeometry;
	std::string errorMessageStr;
	if(!m_webAPI.GetGeometry(Helpers::FStringToString(url), apiData, cameraParameters, sceneGeometry, errorMessageStr))
	{
		errorMessage = Helpers::StringToFString(errorMessageStr);
		return false;
	}

	// Spawn actors from objects:
	auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();;
	renderItems->Clear();
	renderItems->AddGeometry(sceneGeometry);

	return true;
}
bool AAWebAPI::GenerateGeometryByType(EGrammarType grammarType, const FGrammarSpecificData& data, FString& errorMessage)
{
	if(grammarType == EGrammarType::Tutorial)
		return GenerateGeometry(Helpers::StringToFString(URLConstants::TutorialAPI), data, errorMessage);

	return GenerateGeometry(Helpers::StringToFString(URLConstants::OwnGrammarAPI), data, errorMessage);
}

bool AAWebAPI::IsAuthenticated() const
{
	return m_webAPI.IsAuthenticated();
}
