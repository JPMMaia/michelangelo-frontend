#pragma once

#include "FStatus.generated.h"

USTRUCT(BlueprintType)
struct MICHELANGELO_API FStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error")
	bool Success;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Error")
	FString Message;

	FStatus();
	FStatus(bool success, const FString& message);
};
