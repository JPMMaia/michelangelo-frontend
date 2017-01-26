#include "Michelangelo.h"
#include "URenderItem.h"
#include "Unreal/Mesh/AInstancedStaticMeshActor.h"
#include "NonUnreal/MichelangeloAPI/ObjectGeometry.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"

#include <Materials/MaterialInstanceDynamic.h>
#include <Materials/MaterialInstance.h>

using namespace MichelangeloAPI;

URenderItem* URenderItem::Create(const SceneGeometry& sceneGeometry, const ObjectGeometry& objectGeometry)
{
	auto output = NewObject<URenderItem>();

	if (objectGeometry.GetType() == ObjectGeometry::Type::StaticMesh)
	{
		// Create static mesh actor:
		output.m_meshActor = AInstancedStaticMeshActor::CreateFromGeometry(objectGeometry);

		// Add instance:
		output.m_meshActor->AddInstance(objectGeometry);
	}
	else if(objectGeometry.GetType() == ObjectGeometry::Type::ProceduralMesh)
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

	// Set mesh name:
	output.m_meshName = objectGeometry.GetName();

	{
		// Create material instance:
		output.m_material = output.m_meshActor->CreateDynamicMaterialInstance();

		// Get material data:
		auto materialIndex = objectGeometry.GetMaterialIndex();
		const auto& materialData = sceneGeometry.GetMaterials()[materialIndex];
		output->m_materialIndex = objectGeometry.GetMaterialIndex();
		
		// Set diffuse albedo:
		const auto& diffuseAlbedo = materialData.GetDiffuseAlbedo();
		output.m_material->SetVectorParameterValueInternal("DiffuseAlbedo", FLinearColor(diffuseAlbedo[0], diffuseAlbedo[1], diffuseAlbedo[2], diffuseAlbedo[3]));

		// Set ambient intensity:
		const auto& ambientIntensity = materialData.GetAmbientIntensity();
		output.m_material->SetVectorParameterValueInternal("AmbientIntensity", FLinearColor(ambientIntensity[0], ambientIntensity[1], ambientIntensity[2], ambientIntensity[3]));
	}

	return output;
}

AMeshActor* URenderItem::GetMeshActor() const
{
	return m_meshActor;
}
UMaterialInterface* URenderItem::GetMaterial() const
{
	return m_material;
}
const FString& URenderItem::GetMeshName() const
{
	return m_meshName;
}
size_t URenderItem::GetMaterialIndex() const
{
	return m_materialIndex;
}
