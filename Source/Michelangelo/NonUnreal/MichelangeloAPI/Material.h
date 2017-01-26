#pragma once

#include "NonUnreal/nlohmann/JSON/json.hpp"

#include <array>

namespace MichelangeloAPI
{
	class Material
	{
	public:
		static Material CreateFromJSON(const nlohmann::json& materialJson);

		const std::array<float, 4>& GetDiffuseAlbedo() const;
		const std::array<float, 4>& GetAmbientIntensity() const;

	private:
		std::array<float, 4> m_diffuseAlbedo;
		std::array<float, 4> m_ambientIntensity;
	};
}
