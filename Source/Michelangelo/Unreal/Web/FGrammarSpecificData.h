#pragma once

#include "FGrammarSpecificData.generated.h"

namespace MichelangeloAPI {
	struct GrammarSpecificData;
}

USTRUCT(BlueprintType)
struct MICHELANGELO_API FGrammarSpecificData
{
	GENERATED_BODY()

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

	FGrammarSpecificData();

	static FGrammarSpecificData FromGrammarSpecificData(const MichelangeloAPI::GrammarSpecificData& object);
};
