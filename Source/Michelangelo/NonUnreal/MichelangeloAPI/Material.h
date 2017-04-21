#pragma once

#include "NonUnreal/nlohmann/JSON/json.hpp"

#include <array>

namespace MichelangeloAPI
{
	class Material
	{
	public:
		static Material CreateFromJSON(const nlohmann::json& materialJson);

	public:
		Material();

		const std::array<float, 4>& GetDiffuseAlbedo() const;
		const std::array<float, 4>& GetAmbientIntensity() const;
		const std::array<float, 4>& GetSpecularIntensity() const;
		float GetShininess() const;

	private:
		std::array<float, 4> m_diffuseAlbedo;
		std::array<float, 4> m_ambientIntensity;
		std::array<float, 4> m_specularIntensity;
		float m_shininess;
	};
}
