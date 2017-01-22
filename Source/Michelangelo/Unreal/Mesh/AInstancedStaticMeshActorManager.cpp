#include "Michelangelo.h"
#include "Unreal/Mesh/AInstancedStaticMeshActorManager.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "NonUnreal/Common/Helpers.h"
#include "Unreal/Common/UnrealHelpers.h"

using namespace Common;
using namespace MichelangeloAPI;

AInstancedStaticMeshActorManager::AInstancedStaticMeshActorManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AInstancedStaticMeshActorManager::AddGeometry(const ObjectGeometry& geometry)
{
	switch(geometry.GetType())
	{
	case ObjectGeometry::Type::StaticMesh: 
		AddStaticMeshGeometry(geometry);
		break;

	case ObjectGeometry::Type::ProceduralMesh: 
		AddProceduralMeshGeometry(geometry);
		break;

	default: 
		break;
	}
}
void AInstancedStaticMeshActorManager::AddStaticMeshGeometry(const MichelangeloAPI::ObjectGeometry& geometry)
{
	auto name = Helpers::StringToFString(geometry.GetName());

	// If the actor doesn't exist:
	if (this->InstancedStaticMeshActors.Find(name) == nullptr)
	{
		// Instantiate actor:
		FActorSpawnParameters parameters;
		parameters.Name = FName(*name);
		parameters.Owner = this;
		auto actor = GetWorld()->SpawnActor<AInstancedStaticMeshActor>(AInstancedStaticMeshActor::StaticClass(), parameters);

		// Set static mesh:
		auto staticMeshGenerator = UGameDataSingletonLibrary::GetGameDataSingleton()->GetStaticMeshGenerator();
		actor->InstancedStaticMeshComponent->SetStaticMesh(staticMeshGenerator->GetStaticMesh(name));

		// Add to map:
		this->InstancedStaticMeshActors.Add(name, actor);
	}

	// Create world transform:
	auto matrix = Helpers::ArrayToMatrix(geometry.GetTransform());
	matrix = Helpers::MichelangeloToUnrealPrimitiveTransform(matrix);
	auto worldTransform = FTransform(matrix);

	// Add instance:
	auto actor = *InstancedStaticMeshActors.Find(Helpers::StringToFString(geometry.GetName()));
	actor->InstancedStaticMeshComponent->AddInstanceWorldSpace(worldTransform);
}
void AInstancedStaticMeshActorManager::AddProceduralMeshGeometry(const MichelangeloAPI::ObjectGeometry& geometry)
{
	// TODO implement instancing:
	auto name = Helpers::StringToFString(geometry.GetName() + std::to_string(InstancedProceduralMeshActors.Num()));

	// If the key already exists:
	if (this->InstancedProceduralMeshActors.Find(name) != nullptr)
		ThrowEngineException(L"Key already exists.");

	// Instantiate actor:
	FActorSpawnParameters parameters;
	parameters.Name = FName(*name);
	parameters.Owner = this;
	auto actor = GetWorld()->SpawnActor<AInstancedProceduralMeshActor>(AInstancedProceduralMeshActor::StaticClass(), parameters);

	// Create mesh section:
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

		actor->CreateMeshSection(vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}

	// Add to map:
	this->InstancedProceduralMeshActors.Add(name, actor);
}