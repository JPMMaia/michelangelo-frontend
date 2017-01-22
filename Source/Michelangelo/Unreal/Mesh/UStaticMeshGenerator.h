#pragma once

#include "UStaticMeshGenerator.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UStaticMeshGenerator : public UObject
{
	GENERATED_BODY()

public:
	explicit UStaticMeshGenerator(const FObjectInitializer& ObjectInitializer);

	UStaticMesh* GetStaticMesh(const FString& name);

	TMap<FString, UStaticMesh*> StaticMeshes;
};