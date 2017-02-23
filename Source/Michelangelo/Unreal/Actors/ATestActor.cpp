#include "Michelangelo.h"
#include "Unreal/Actors/ATestActor.h"
#include "Unreal/Web/AWebAPI.h"

#include <fstream>
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"

using namespace std;

ATestActor::ATestActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	// Create a default root component:
	RootComponent = objectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RootComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void ATestActor::BeginPlay()
{
	//{
	FActorSpawnParameters parameters;
	parameters.Name = FName(TEXT("GeneratedProceduralMesh"));
	parameters.Owner = this;
	auto actor = GetWorld()->SpawnActor<AAWebAPI>(AAWebAPI::StaticClass(), parameters);

	// Read geometry from file:
	/*MichelangeloAPI::SceneGeometry sceneGeometry;
	{
		ifstream fileStream(L"TestClock.json", ios::in);

		nlohmann::json json;
		fileStream >> json;

		sceneGeometry = MichelangeloAPI::SceneGeometry::CreateFromJson(json);
	}*/

	// TODO add authentication
	actor->Authenticate("jpmmaia@gmail.com", "LijOZ5nCDHSs6adrI3fSfKPYQrlf8V6yZCcih4WUAEqQeCsCLfWgGCTCDVDXmCxj", false);
	//auto grammars = actor->GetGrammarsByType(EGrammarType::Tutorial);
	//auto grammarData = actor->GetGrammarSpecificDataByType(EGrammarType::Tutorial, grammars[0].ID);
	//actor->GenerateGeometryByType(EGrammarType::Tutorial, grammarData);

	/*		auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();;
			renderItems->Clear();
			renderItems->AddGeometry(sceneGeometry);*/

	{
		DrawDebugLine(
			GetWorld(),
			FVector(0.0f, 0.0f, 0.0f),
			FVector(0.0f, 100.0f, 0.0f),
			FColor(255, 0, 0),
			true, -1, 0,
			12.333
		);

		DrawDebugLine(
			GetWorld(),
			FVector(0.0f, 0.0f, 0.0f),
			FVector(0.0f, 0.0f, 100.0f),
			FColor(0, 255, 0),
			true, -1, 0,
			12.333
		);

		DrawDebugLine(
			GetWorld(),
			FVector(0.0f, 0.0f, 0.0f),
			FVector(-100.0f, 0.0f, 0.0f),
			FColor(0, 0, 255),
			true, -1, 0,
			12.333
		);
	}




	/*{
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
	}*/

}
