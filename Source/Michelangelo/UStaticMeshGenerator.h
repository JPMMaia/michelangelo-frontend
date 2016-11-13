#pragma once

#include "UStaticMeshGenerator.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UStaticMeshGenerator : public UObject
{
	GENERATED_BODY()

public:
	explicit UStaticMeshGenerator(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = StaticMeshAssets)
	UStaticMesh* CubeStaticMesh;

	UPROPERTY(EditAnywhere, Category = StaticMeshAssets)
	UStaticMesh* SphereStaticMesh;

	UPROPERTY(EditAnywhere, Category = StaticMeshAssets)
	UStaticMesh* CylinderStaticMesh;

	UPROPERTY(EditAnywhere, Category = StaticMeshAssets)
	UStaticMesh* ConeStaticMesh;
};