#pragma once

#include <Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>

#include "InstancedStaticSkewedMeshComponent.generated.h"

UCLASS()
class UInstancedStaticSkewedMeshComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Components|InstancedStaticSkewedMesh")
	int32 AddInstanceMatrix(const FMatrix& InInstanceTransform);

	UFUNCTION(BlueprintCallable, Category = "Components|InstancedStaticSkewedMesh")
	int32 AddInstanceWorldSpaceMatrix(const FMatrix& InWorldTransform);

private:
	void SetupNewInstanceDataMatrix(FInstancedStaticMeshInstanceData& InOutNewInstanceData, int32 InInstanceIndex, const FMatrix& InInstanceTransform);

private:
	static int32 GetNumShapes(UBodySetup* BodySetup);
	static int32 GetAggregateIndex(int32 BodyIndex, int32 NumShapes);
};
