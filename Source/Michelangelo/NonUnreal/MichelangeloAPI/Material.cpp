#include "Michelangelo.h"
#include "Material.h"

using namespace MichelangeloAPI;

Material Material::CreateFromJSON(const nlohmann::json& materialJson)
{
	Material material;

	// Ambient:
	{
		auto colorArray = materialJson.at("Ambient");
		std::copy(colorArray.begin(), colorArray.end(), material.m_ambientIntensity.begin());
	}

	// Diffuse:
	{
		auto colorArray = materialJson.at("Diffuse");
		std::copy(colorArray.begin(), colorArray.end(), material.m_diffuseAlbedo.begin());
	}

	return material;
}
