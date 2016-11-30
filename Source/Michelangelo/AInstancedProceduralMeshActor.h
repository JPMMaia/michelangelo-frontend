#pragma once

#include <GameFramework/Actor.h>
#include "ProceduralMeshComponent.h"
#include "AInstancedProceduralMeshActor.generated.h"

UCLASS()
class MICHELANGELO_API AInstancedProceduralMeshActor : public AActor
{
	GENERATED_BODY()

public:
	explicit AInstancedProceduralMeshActor(const FObjectInitializer& objectInitializer);

	UFUNCTION(BlueprintCallable, Category = "InstancedProceduralMeshActor")
	void CreateMeshSection(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision);

	UPROPERTY(EditAnywhere, Category = "InstancedProceduralMeshActor")
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "InstancedProceduralMeshActor")
	int32 SectionCount;
};
