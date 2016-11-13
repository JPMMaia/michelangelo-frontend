// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "AWebAPI.h"
#include "MichelangeloAPI/URLConstants.h"
#include "UGameDataSingletonLibrary.h"
#include "UGameDataSingleton.h"
#include "AInstancedStaticMeshActorManager.h"

using namespace Common;
using namespace MichelangeloAPI;

AAWebAPI::AAWebAPI()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AAWebAPI::Authenticate(const FString& email, const FString& password, bool rememberMe)
{	
	return m_webAPI.Authenticate(Helpers::FStringToString(email), Helpers::FStringToString(password), rememberMe);
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

	auto grammarData = m_webAPI.GetGrammarSpecificData(Helpers::FStringToString(url), Helpers::FStringToString(id));

	FGrammarSpecificData output;
	output.ID = Helpers::StringToFString(grammarData.ID);
	output.Name = Helpers::StringToFString(grammarData.Name);
	output.Type = Helpers::StringToFString(grammarData.Type);
	output.Code = Helpers::StringToFString(grammarData.Code);
	output.Shared = grammarData.Shared;
	output.IsOwner = grammarData.IsOwner;

	return output;
}

FGrammarSpecificData AAWebAPI::GetGrammarSpecificDataByType(EGrammarType grammarType, const FString& id) const
{
	FGrammarSpecificData output;

	switch (grammarType)
	{
	case EGrammarType::Own:
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::OwnGrammarAPI), id);
		break;

	case EGrammarType::Shared:
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::SharedGrammarAPI), id);
		break;

	case EGrammarType::Tutorial:
		output = GetGrammarSpecificData(Helpers::StringToFString(URLConstants::TutorialAPI), id);
		break;

	default:
		break;
	}

	return output;
}

void AAWebAPI::GenerateGeometry(const FString& url, const FGrammarSpecificData& data)
{
	GrammarSpecificData apiData;
	apiData.ID = Helpers::FStringToString(data.ID);
	apiData.Name = Helpers::FStringToString(data.Name);
	apiData.Type = Helpers::FStringToString(data.Type);
	apiData.Code = Helpers::FStringToString(data.Code);
	apiData.Shared = data.Shared;
	apiData.IsOwner = data.IsOwner;

	auto sceneGeometry = m_webAPI.GetGeometry(Helpers::FStringToString(url), apiData);

	// Spawn actors from objects:
	{
		auto instancedStaticMeshActorManager = UGameDataSingletonLibrary::GetGameDataSingleton()->GetInstancedStaticMeshActorManager(GetWorld());

		for (auto& object : sceneGeometry.Objects)
		{
			auto worldTransform = FTransform(Helpers::ArrayToMatrix(object.Transform));

			auto actor = instancedStaticMeshActorManager->GetInstancedStaticMeshActor(Helpers::StringToFString(object.Name));
			actor->InstancedStaticMeshComponent->AddInstanceWorldSpace(worldTransform);
		}
	}
}
void AAWebAPI::GenerateGeometryByType(EGrammarType grammarType, const FGrammarSpecificData& data)
{
	switch (grammarType)
	{
	case EGrammarType::Own:
		GenerateGeometry(Helpers::StringToFString(URLConstants::OwnGrammarAPI), data);
		break;

	case EGrammarType::Shared:
		GenerateGeometry(Helpers::StringToFString(URLConstants::SharedGrammarAPI), data);
		break;

	case EGrammarType::Tutorial:
		GenerateGeometry(Helpers::StringToFString(URLConstants::TutorialAPI), data);
		break;

	default:
		break;
	}
}

bool AAWebAPI::IsAuthenticated() const
{
	return m_webAPI.IsAuthenticated();
}
