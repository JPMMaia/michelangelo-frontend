#pragma once

#include "FGrammarSpecificData.generated.h"

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
};
