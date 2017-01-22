#pragma once

#include "FObjectGeometry.generated.h"

USTRUCT(BlueprintType)
struct MICHELANGELO_API FObjectGeometry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Geometry")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Geometry")
	TArray<float> Coordinates;

	FObjectGeometry();
};
