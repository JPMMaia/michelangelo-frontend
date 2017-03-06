#pragma once

#include <Object.h>
#include <string>

#include "EGrammarType.h"
#include "GrammarSpecificData.generated.h"

namespace MichelangeloAPI {
	struct GrammarSpecificData;
}

UCLASS(BlueprintType)
class MICHELANGELO_API UGrammarSpecificData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Specific Data")
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Specific Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Specific Data")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Specific Data")
	FString Code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Specific Data")
	bool Shared;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Specific Data")
	bool IsOwner;

public:
	static UGrammarSpecificData* FromApiData(const MichelangeloAPI::GrammarSpecificData& object);

public:
	UGrammarSpecificData();

	MichelangeloAPI::GrammarSpecificData ToApiData() const;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	EGrammarType GetGrammarType() const;
};
