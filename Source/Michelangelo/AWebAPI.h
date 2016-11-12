// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MichelangeloAPI/WebAPI.h"
#include "FGrammarData.h"
#include "FGrammarSpecificData.h"
#include "EGrammarType.h"
#include "Common/Helpers.h"
#include "AWebAPI.generated.h"

UCLASS()
class MICHELANGELO_API AAWebAPI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWebAPI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category="WebAPI")
	bool Authenticate(const FString& email, const FString& password, bool rememberMe);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	TArray<FGrammarData> GetGrammars(const FString& url) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	TArray<FGrammarData> GetGrammarsByType(EGrammarType grammarType) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	FGrammarSpecificData GetGrammarSpecificData(const FString& url, const FString& id) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	FGrammarSpecificData GetGrammarSpecificDataByType(EGrammarType grammarType, const FString& id) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	bool IsAuthenticated() const;

private:
	MichelangeloAPI::WebAPI m_webAPI;
};
