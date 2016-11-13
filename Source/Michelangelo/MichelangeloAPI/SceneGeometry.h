#pragma once

#include <array>
#include <string>
#include <vector>

namespace MichelangeloAPI
{
	struct ObjectGeometry
	{
		std::string Name;
		std::array<float, 16> Transform;
	};

	struct SceneGeometry
	{
		std::vector<ObjectGeometry> Objects;
	};
}