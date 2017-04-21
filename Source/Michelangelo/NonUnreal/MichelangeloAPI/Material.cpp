#include "Michelangelo.h"
#include "Material.h"
#include "NonUnreal/Common/Helpers.h"

using namespace Common;
using namespace MichelangeloAPI;

Material Material::CreateFromJSON(const nlohmann::json& materialJson)
{
	Material material;

	Helpers::ParseColor(materialJson, "Diffuse", material.m_diffuseAlbedo);
	Helpers::ParseColor(materialJson, "Ambient", material.m_ambientIntensity);
	Helpers::ParseColor(materialJson, "Specular", material.m_specularIntensity);
	Helpers::ParseFloat(materialJson, "Shininess", material.m_shininess);
	
	return material;
}

Material::Material() :
	m_diffuseAlbedo({ 0.0f, 0.0f, 0.0f, 1.0f }),
	m_ambientIntensity({ 0.0f, 0.0f, 0.0f, 1.0f }),
	m_specularIntensity({ 0.5f, 0.5f, 0.5f, 0.5f }),
	m_shininess(128.0f)
{
}

const std::array<float, 4>& Material::GetDiffuseAlbedo() const
{
	return m_diffuseAlbedo;
}
const std::array<float, 4>& Material::GetAmbientIntensity() const
{
	return m_ambientIntensity;
}
const std::array<float, 4>& Material::GetSpecularIntensity() const
{
	return m_specularIntensity;
}
float Material::GetShininess() const
{
	return m_shininess;
}

