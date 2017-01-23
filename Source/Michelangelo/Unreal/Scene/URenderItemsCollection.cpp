#include "Michelangelo.h"
#include "URenderItemsCollection.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"

using namespace Common;
using namespace MichelangeloAPI;

void URenderItemsCollection::AddGeometry(const SceneGeometry& sceneGeometry)
{
	HandleMaterials(sceneGeometry);
	HandleObjects(sceneGeometry);
}

void URenderItemsCollection::HandleMaterials(const MichelangeloAPI::SceneGeometry& sceneGeometry)
{
	// Get the basic material loaded from a blueprint:
	auto materialLoader = UGameDataSingletonLibrary::GetGameDataSingleton()->GetMaterialLoader();
	auto basicMaterial = materialLoader->GetMaterial("BasicMaterial");

	for (const auto& material : sceneGeometry.GetMaterials())
	{
		
	}
}
void URenderItemsCollection::HandleObjects(const SceneGeometry& sceneGeometry)
{
	for(const auto& object : sceneGeometry.GetObjects())
	{
		if (object.GetType() == ObjectGeometry::Type::StaticMesh)
			CreateStaticMesh(object);
		else if (object.GetType() == ObjectGeometry::Type::ProceduralMesh)
			CreateProceduralMesh(object);
	}
}

void URenderItemsCollection::CreateStaticMesh(const ObjectGeometry& objectGeometry)
{
	auto name = Helpers::StringToFString(objectGeometry.GetName());

	// Find actor:
	AInstancedStaticMeshActor* pActor;
	auto ppActor = m_staticMeshActors.Find(name);

	// If the actor exists:
	if(ppActor != nullptr)
	{
		pActor = *ppActor;
	}
	// If the actor doesn't exist:
	else
	{
		// Create static mesh actor:
		pActor = AInstancedStaticMeshActor::CreateFromGeometry(objectGeometry);

		// Add to map:
		m_staticMeshActors.Add(name, pActor);
	}

	// Add instance:
	pActor->AddInstance(objectGeometry);
}
void URenderItemsCollection::CreateProceduralMesh(const MichelangeloAPI::ObjectGeometry& objectGeometry)
{
	// TODO

	/*auto name = Helpers::StringToFString(geometry.GetName() + std::to_string(InstancedProceduralMeshActors.Num()));

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

	// Set world transform:
	auto matrix = Helpers::ArrayToMatrix(geometry.GetTransform());
	matrix = Helpers::MichelangeloToUnrealPrimitiveTransform(matrix);
	auto worldTransform = FTransform(matrix);
	actor->SetActorTransform(worldTransform);

	// Add to map:
	this->InstancedProceduralMeshActors.Add(name, actor);*/

}
