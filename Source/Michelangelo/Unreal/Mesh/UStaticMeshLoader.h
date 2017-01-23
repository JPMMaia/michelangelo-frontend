#pragma once

#include "UStaticMeshGenerator.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UStaticMeshLoader : public UObject
{
	GENERATED_BODY()

public:
	explicit UStaticMeshLoader(const FObjectInitializer& ObjectInitializer);

	UStaticMesh* GetStaticMesh(const FString& name);

	TMap<FString, UStaticMesh*> StaticMeshes;
};