#pragma once

#include <UObject.h>
#include "UMaterialLoader.generated.h"

UCLASS()
class MICHELANGELO_API UMaterialLoader : public UObject
{
GENERATED_BODY()

public:
	explicit UMaterialLoader(const FObjectInitializer& ObjectInitializer);

	UMaterialInterface* GetMaterial(const FString& name);

private:
	void AddMaterial(const FString& name);

private:
	TMap<FString, UMaterialInterface*> Materials;
};
