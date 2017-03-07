// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "UWebAPI.h"
#include "NonUnreal/MichelangeloAPI/URLConstants.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/UGameDataSingleton.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/CameraParameters.h"

using namespace Common;
using namespace MichelangeloAPI;

UWebAPI::UWebAPI()
{
	m_getGrammarsURLs.Add(EGrammarType::Own, Helpers::StringToFString(URLConstants::OwnGrammarAPI));
	m_getGrammarsURLs.Add(EGrammarType::Shared, Helpers::StringToFString(URLConstants::SharedGrammarAPI));
	m_getGrammarsURLs.Add(EGrammarType::Tutorial, Helpers::StringToFString(URLConstants::TutorialAPI));
}

FStatus UWebAPI::Authenticate(const FString& email, const FString& password, bool rememberMe)
{
	if (email.IsEmpty())
		return FStatus(false, TEXT("Email field must not be empty!"));

	if (password.IsEmpty())
		return FStatus(false, TEXT("Password field must not be empty!"));

	if (rememberMe)
	{
		// Save email for future use:
		auto gameDataSingleton = UGameDataSingleton::Get();
		gameDataSingleton->SetSavedEmail(email);
	}

/*	auto error = m_webAPI.Authenticate(Helpers::FStringToString(email), Helpers::FStringToString(password), false);

	switch (error)
	{
	case NativeWebAPI::LoginError::None:
		return FStatus(true, TEXT("Success!"));

	case NativeWebAPI::LoginError::WrongCredentials:
		return FStatus(false, TEXT("Wrong credentials!"));

	default:
		return FStatus(false, "Unexpected error.");
	}*/

	return FStatus(false, TEXT("Success!"));
}

void UWebAPI::LogOut()
{
	m_webAPI.LogOut();
}

UGrammarSpecificData* UWebAPI::CreateNewGrammar()
{
	return UGrammarSpecificData::FromApiData(m_webAPI.CreateNewGrammar());
}
void UWebAPI::DeleteGrammar(const FString& id)
{
	m_webAPI.DeleteGrammar(Helpers::FStringToString(id));
}
void UWebAPI::ShareGrammar(const FString& id, bool share)
{
	m_webAPI.ShareGrammar(Helpers::FStringToString(id), share);
}

TArray<UGrammarSpecificData*> UWebAPI::GetGrammars(const FString& url) const
{
	if (!IsAuthenticated())
		return TArray<UGrammarSpecificData*>();

	auto grammarsData = m_webAPI.GetGrammars(Helpers::FStringToString(url));

	TArray<UGrammarSpecificData*> output;
	for (auto& grammarData : grammarsData)
	{
		output.Add(UGrammarSpecificData::FromApiData(grammarData));
	}

	return output;
}
TArray<UGrammarSpecificData*> UWebAPI::GetGrammarsByType(EGrammarType grammarType) const
{
	TArray<UGrammarSpecificData*> output;

	switch (grammarType)
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

UGrammarSpecificData* UWebAPI::GetGrammarSpecificData(const FString& url, const FString& id) const
{
	if (!IsAuthenticated())
		return NewObject<UGrammarSpecificData>();

	return UGrammarSpecificData::FromApiData(m_webAPI.GetGrammarSpecificData(Helpers::FStringToString(url), Helpers::FStringToString(id)));
}

UGrammarSpecificData* UWebAPI::GetGrammarSpecificDataByType(EGrammarType grammarType, const FString& id) const
{
	UGrammarSpecificData* output;

	switch (grammarType)
	{
	case EGrammarType::Own:
	case EGrammarType::Shared:
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::OwnGrammarAPI), id);
		break;

	case EGrammarType::Tutorial:
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::TutorialAPI), id);
		break;

	default:
		output = NewObject<UGrammarSpecificData>();
		break;
	}

	return output;
}

bool UWebAPI::GenerateGeometry(const FString& url, const UGrammarSpecificData* data, FString& errorMessage)
{
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

	// Perform request and get the geometry:
	SceneGeometry sceneGeometry;
	std::string errorMessageStr;
	{
		// Perform request:
		bool sucess = m_webAPI.GetGeometry(Helpers::FStringToString(url), data->ToApiData(), cameraParameters, sceneGeometry, errorMessageStr);
		
		// Set output error message:
		errorMessage = Helpers::StringToFString(errorMessageStr);

		// If it wasn't success, then no geometry was generated:
		if (!sucess)
			return false;
	}

	// Spawn actors from objects:
	auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();
	renderItems->Clear();
	renderItems->AddGeometry(sceneGeometry);

	return true;
}
bool UWebAPI::GenerateGeometryByType(EGrammarType grammarType, const UGrammarSpecificData* data, FString& errorMessage)
{
	if (grammarType == EGrammarType::Tutorial)
		return GenerateGeometry(Helpers::StringToFString(URLConstants::TutorialAPI), data, errorMessage);

	return GenerateGeometry(Helpers::StringToFString(URLConstants::OwnGrammarAPI), data, errorMessage);
}

bool UWebAPI::IsAuthenticated() const
{
	return m_webAPI.GetSessionData().IsAuthenticated();
}

MichelangeloAPI::NativeWebAPI& UWebAPI::GetNativeWebAPI()
{
	return m_webAPI;
}

const FString& UWebAPI::SelectGetGrammarsURL(EGrammarType type) const
{
	return m_getGrammarsURLs[type];
}