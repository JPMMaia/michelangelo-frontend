// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NonUnreal/MichelangeloAPI/WebAPI.h"
#include "Unreal/Web/FGrammarData.h"
#include "Unreal/Web/FGrammarSpecificData.h"
#include "Unreal/Web/EGrammarType.h"
#include "NonUnreal/Common/Helpers.h"
#include "Unreal/Common/FStatus.h"
#include "AWebAPI.generated.h"

UCLASS()
class MICHELANGELO_API AAWebAPI : public AActor
{
	GENERATED_BODY()
	
public:	
	AAWebAPI();

	UFUNCTION(BlueprintCallable, Category="WebAPI")
	FStatus Authenticate(const FString& email, const FString& password, bool rememberMe);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	void LogOut();

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	TArray<FGrammarData> GetGrammars(const FString& url) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	TArray<FGrammarData> GetGrammarsByType(EGrammarType grammarType) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	FGrammarSpecificData GetGrammarSpecificData(const FString& url, const FString& id) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	FGrammarSpecificData GetGrammarSpecificDataByType(EGrammarType grammarType, const FString& id) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	void GenerateGeometry(const FString& url, const FGrammarSpecificData& data);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	void GenerateGeometryByType(EGrammarType grammarType, const FGrammarSpecificData& data);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	bool IsAuthenticated() const;

private:
	MichelangeloAPI::WebAPI m_webAPI;
};