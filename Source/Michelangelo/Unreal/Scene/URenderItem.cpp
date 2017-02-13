#include "Michelangelo.h"
#include "URenderItem.h"
#include "Unreal/Mesh/AInstancedStaticMeshActor.h"
#include "Unreal/Mesh/AInstancedProceduralMeshActor.h"
#include "NonUnreal/MichelangeloAPI/ObjectGeometry.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"

#include <Materials/MaterialInstanceDynamic.h>
#include <Materials/MaterialInstance.h>
#include "Unreal/Common/UnrealHelpers.h"


using namespace MichelangeloAPI;

URenderItem* URenderItem::Create(const SceneGeometry& sceneGeometry, const ObjectGeometry& objectGeometry)
{
	auto output = NewObject<URenderItem>();

	if (objectGeometry.GetType() == ObjectGeometry::Type::StaticMesh)
	{
		// Create static mesh actor:
		output->m_meshActor = AInstancedStaticMeshActor::CreateFromGeometry(objectGeometry);
	}
	else if(objectGeometry.GetType() == ObjectGeometry::Type::ProceduralMesh)
	{
		// Create procedural mesh actor:
		output->m_meshActor = AInstancedProceduralMeshActor::CreateFromGeometry(objectGeometry);
	}

	// Add instance:
	output->m_meshActor->AddInstance(objectGeometry);

	// Set mesh name:
	output->m_meshName = Common::Helpers::StringToFString(objectGeometry.GetName());

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
		const auto& specularIntensity = materialData.GetSpecularIntensity();
		output->m_material->SetVectorParameterValue("SpecularIntensity", FLinearColor(specularIntensity[0], specularIntensity[1], specularIntensity[2], specularIntensity[3]));

		// Set shininess:
		auto shininess = materialData.GetShininess();
		output->m_material->SetScalarParameterValue("Shininess", shininess);
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
