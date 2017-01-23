#pragma once

#include "NonUnreal/nlohmann/JSON/json.hpp"

#include <array>

namespace MichelangeloAPI
{
	class Material
	{
	public:
		static Material CreateFromJSON(const nlohmann::json& materialJson);

	private:
		std::array<float, 4> m_diffuseAlbedo;
		std::array<float, 4> m_ambientIntensity;
	};
}
