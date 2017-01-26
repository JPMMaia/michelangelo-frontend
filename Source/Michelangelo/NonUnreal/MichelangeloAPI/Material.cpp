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

		if (colorArray.size() == 3)
			material.m_ambientIntensity[3] = 1.0f;
	}

	// Diffuse:
	{
		auto colorArray = materialJson.at("Diffuse");
		std::copy(colorArray.begin(), colorArray.end(), material.m_diffuseAlbedo.begin());

		if (colorArray.size() == 3)
			material.m_diffuseAlbedo[3] = 1.0f;
	}

	return material;
}

const std::array<float, 4>& Material::GetDiffuseAlbedo() const
{
	return m_diffuseAlbedo;
}
const std::array<float, 4>& Material::GetAmbientIntensity() const
{
	return m_ambientIntensity;
}
