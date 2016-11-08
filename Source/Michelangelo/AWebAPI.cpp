// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "AWebAPI.h"
#include "Common/Helpers.h"
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
		// TODO type
		output.Add(fGrammarData);
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
