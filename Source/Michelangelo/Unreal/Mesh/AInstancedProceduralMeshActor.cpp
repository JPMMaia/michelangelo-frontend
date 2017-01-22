#include "Michelangelo.h"
#include "AInstancedProceduralMeshActor.h"

AInstancedProceduralMeshActor::AInstancedProceduralMeshActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer),
	SectionCount(0)
{
	// Create instanced procedural mesh component:
	ProceduralMeshComponent = objectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralMeshComponent"));

	// Set it as root component:
	RootComponent = ProceduralMeshComponent;

	PrimaryActorTick.bCanEverTick = true;
}

void AInstancedProceduralMeshActor::CreateMeshSection(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision)
{
	ProceduralMeshComponent->CreateMeshSection(++SectionCount, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);
}
