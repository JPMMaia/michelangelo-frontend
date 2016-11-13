#pragma once

#include "FSceneGeometry.generated.h"

USTRUCT(BlueprintType)
struct MICHELANGELO_API FSceneGeometry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Geometry")
	FString ID;

	FSceneGeometry();
};
