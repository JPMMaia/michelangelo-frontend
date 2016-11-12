// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "AWebAPI.h"
#include "MichelangeloAPI/URLConstants.h"

using namespace Common;
using namespace MichelangeloAPI;

// Sets default values
AAWebAPI::AAWebAPI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAWebAPI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWebAPI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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

TArray<FGrammarData> AAWebAPI::GetOwnGrammars() const
{
	return GetGrammars(Helpers::StringToFString(URLConstants::OwnGrammarAPI));
}
TArray<FGrammarData> AAWebAPI::GetSharedGrammars() const
{
	return GetGrammars(Helpers::StringToFString(URLConstants::SharedGrammarAPI));
}
TArray<FGrammarData> AAWebAPI::GetTutorials() const
{
	return GetGrammars(Helpers::StringToFString(URLConstants::TutorialAPI));
}

bool AAWebAPI::IsAuthenticated() const
{
	return m_webAPI.IsAuthenticated();
}
