#pragma once

#include <Object.h>
#include <memory>

#include "Unreal/Web/GrammarSpecificData.h"
#include "Unreal/Web/EGrammarType.h"
#include "Unreal/Common/FStatus.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "UWebAPI.generated.h"

UCLASS()
class MICHELANGELO_API UWebAPI : public UObject
{
	GENERATED_BODY()
	
public:	
	UWebAPI();

	UFUNCTION(BlueprintCallable, Category="WebAPI")
	FStatus Authenticate(const FString& email, const FString& password, bool rememberMe);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	void LogOut();

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	UGrammarSpecificData* CreateNewGrammar();

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	void DeleteGrammar(const FString& id);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	void ShareGrammar(const FString& id, bool share);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	TArray<UGrammarSpecificData*> GetGrammars(const FString& url) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	TArray<UGrammarSpecificData*> GetGrammarsByType(EGrammarType grammarType) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	UGrammarSpecificData* GetGrammarSpecificData(const FString& url, const FString& id) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	UGrammarSpecificData* GetGrammarSpecificDataByType(EGrammarType grammarType, const FString& id) const;

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	bool GenerateGeometry(const FString& url, const UGrammarSpecificData* data, FString& errorMessage);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	bool GenerateGeometryByType(EGrammarType grammarType, const UGrammarSpecificData* data, FString& errorMessage);

	UFUNCTION(BlueprintCallable, Category = "WebAPI")
	bool IsAuthenticated() const;

	MichelangeloAPI::NativeWebAPI& GetNativeWebAPI();

	const FString& SelectGetGrammarsURL(EGrammarType type) const;

private:
	MichelangeloAPI::NativeWebAPI m_webAPI;
	TMap<EGrammarType, FString> m_getGrammarsURLs;
};
