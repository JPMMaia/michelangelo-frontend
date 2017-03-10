#include "Michelangelo.h"
#include "URenderItem.h"
#include "Unreal/Mesh/AInstancedStaticMeshActor.h"
#include "Unreal/Mesh/AInstancedProceduralMeshActor.h"
#include "NonUnreal/MichelangeloAPI/ObjectGeometry.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"

#include <Materials/MaterialInstanceDynamic.h>
#include <Materials/MaterialInstance.h>
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/Common/EngineException.h"

using namespace Common;
using namespace MichelangeloAPI;

URenderItem* URenderItem::Create(const SceneGeometry& sceneGeometry, const ObjectGeometry& objectGeometry)
{
	auto output = NewObject<URenderItem>();
	output->AddToRoot();

	if (objectGeometry.GetType() == ObjectGeometry::Type::StaticMesh)
	{
		// Create static mesh actor:
		output->m_meshActor = AInstancedStaticMeshActor::CreateFromGeometry(objectGeometry);

		// Set mesh name:
		output->m_meshName = Helpers::StringToFString(objectGeometry.GetName());
	}
	else if(objectGeometry.GetType() == ObjectGeometry::Type::ProceduralMesh)
	{
		// Create procedural mesh actor:
		output->m_meshActor = AInstancedProceduralMeshActor::CreateFromGeometry(objectGeometry);
		
		// Set mesh name:
		output->m_meshName = output->m_meshActor->GetName();
	}
	else
	{
		throw std::invalid_argument("Geometry type not supported");
	}

	// Add instance:
	output->m_meshActor->AddInstance(objectGeometry);

	{
		// Create material instance:
		output->m_material = output->m_meshActor->CreateDynamicMaterialInstance();

		// Get material data:
		auto materialIndex = objectGeometry.GetMaterialIndex();
		const auto& materialData = sceneGeometry.GetMaterials()[materialIndex];
		output->m_materialIndex = objectGeometry.GetMaterialIndex();
		
		// Set diffuse albedo:
		const auto& diffuseAlbedo = materialData.GetDiffuseAlbedo();
		output->m_material->SetVectorParameterValue("DiffuseAlbedo", FLinearColor(diffuseAlbedo[0], diffuseAlbedo[1], diffuseAlbedo[2], diffuseAlbedo[3]));

		// Set ambient intensity:
		const auto& ambientIntensity = materialData.GetAmbientIntensity();
		output->m_material->SetVectorParameterValue("AmbientIntensity", FLinearColor(ambientIntensity[0], ambientIntensity[1], ambientIntensity[2], ambientIntensity[3]));

		// Set specular intensity:
		const auto& specularColor = materialData.GetSpecularIntensity();
		auto specularIntensity = (specularColor[0] + specularColor[1] + specularColor[2]) / 3.0f;
		output->m_material->SetScalarParameterValue("SpecularIntensity", specularIntensity);

		// Set shininess:
		auto shininess = materialData.GetShininess() / 256.0f;
		output->m_material->SetScalarParameterValue("Shininess", shininess);
	}

	return output;
}

void URenderItem::BeginDestroy()
{
	Super::BeginDestroy();

	Helpers::DestroyObject(m_meshActor);
	Helpers::DestroyObject(m_material);
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
