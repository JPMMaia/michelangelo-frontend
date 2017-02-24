#include "Michelangelo.h"
#include "AInstancedProceduralMeshActor.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/ObjectGeometry.h"

using namespace Common;

size_t AInstancedProceduralMeshActor::s_id = 0;

AInstancedProceduralMeshActor* AInstancedProceduralMeshActor::CreateFromGeometry(const MichelangeloAPI::ObjectGeometry& geometry)
{
	auto gameDataSingleton = UGameDataSingleton::Get();

	// Get spawner:
	auto spawner = gameDataSingleton->GetSpawner();

	// Get name of geometry:
	auto staticMeshName = Common::Helpers::StringToFString(geometry.GetName());

	// Instantiate actor:
	FActorSpawnParameters parameters;
	auto uniqueName = staticMeshName;
	uniqueName.AppendInt(s_id++);
	parameters.Name = FName(*uniqueName);
	parameters.Owner = spawner;
	auto actor = spawner->GetWorld()->SpawnActor<AInstancedProceduralMeshActor>(AInstancedProceduralMeshActor::StaticClass(), parameters);

	// Create mesh section:
	actor->CreateMeshSection(geometry);

	return actor;
}

AInstancedProceduralMeshActor::AInstancedProceduralMeshActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer),
	m_sectionCount(0)
{
	// Create instanced procedural mesh component:
	m_proceduralMeshComponent = objectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralMeshComponent"));

	// Set it as root component:
	RootComponent = m_proceduralMeshComponent;

	PrimaryActorTick.bCanEverTick = true;
}

void AInstancedProceduralMeshActor::AddInstance(const MichelangeloAPI::ObjectGeometry& instanceData)
{
	// Set world transform:
	//auto matrix = Helpers::ArrayToMatrix(instanceData.GetTransform());
	//matrix = Helpers::MichelangeloToUnrealGeneralTransform(matrix);
	
	//auto worldTransform = FTransform(matrix);
	auto worldTransform = FTransform::Identity;
	this->SetActorTransform(worldTransform);
}
UMaterialInstanceDynamic* AInstancedProceduralMeshActor::CreateDynamicMaterialInstance()
{
	// Get a source material loaded from a blueprint:
	auto materialLoader = UGameDataSingleton::Get()->GetMaterialLoader();
	auto sourceMaterial = materialLoader->GetMaterial("BasicMaterial");

	// Create material instance:
	return m_proceduralMeshComponent->CreateDynamicMaterialInstance(0, sourceMaterial);
}

void AInstancedProceduralMeshActor::CreateMeshSection(const MichelangeloAPI::ObjectGeometry& geometry)
{
	const auto& geometryVertices = geometry.GetVertices();

	TArray<FVector> vertices;
	auto vertexIterator = geometryVertices.begin();
	while (vertexIterator != geometryVertices.end())
		vertices.Add(FVector(*vertexIterator++, *vertexIterator++, *vertexIterator++));

	const auto& geometryIndices = geometry.GetIndices();
	TArray<int32> triangles;
	triangles.AddUninitialized(geometryIndices.size());
	FMemory::Memcpy(triangles.GetData(), geometryIndices.data(), geometryIndices.size() * sizeof(int32));

	m_proceduralMeshComponent->CreateMeshSection(++m_sectionCount, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}
