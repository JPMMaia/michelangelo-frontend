#pragma once

#include "FGrammarData.generated.h"

USTRUCT(BlueprintType)
struct MICHELANGELO_API FGrammarData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Data")
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grammar Data")
	FString Type;

	FGrammarData();
};
