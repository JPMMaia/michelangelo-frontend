#include "Michelangelo.h"
#include "ATestActor.h"
#include "AWebAPI.h"

ATestActor::ATestActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{	
	// Create a default root component:
	RootComponent = objectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RootComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void ATestActor::BeginPlay()
{
	/*{
		FActorSpawnParameters parameters;
		parameters.Name = FName(TEXT("GeneratedProceduralMesh"));
		parameters.Owner = this;
		auto actor = GetWorld()->SpawnActor<AAWebAPI>(AAWebAPI::StaticClass(), parameters);

		// TODO add authentication
		actor->Authenticate("", "", false);
		auto grammars = actor->GetGrammarsByType(EGrammarType::Tutorial);
		auto grammarData = actor->GetGrammarSpecificDataByType(EGrammarType::Tutorial, grammars[0].ID);
		actor->GenerateGeometryByType(EGrammarType::Tutorial, grammarData);
	}*/


	/*
	{
	FActorSpawnParameters parameters;
	parameters.Name = FName(TEXT("GeneratedProceduralMesh"));
	parameters.Owner = this;
	auto actor = GetWorld()->SpawnActor<AInstancedProceduralMeshActor>(AInstancedProceduralMeshActor::StaticClass(), parameters);

	{
	TArray<FVector> vertices;
	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(0, 0, 100));

	TArray<int32> triangles;
	triangles.Add(0);
	triangles.Add(1);
	triangles.Add(2);

	actor->CreateMeshSection(vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
	}
	*/
}
