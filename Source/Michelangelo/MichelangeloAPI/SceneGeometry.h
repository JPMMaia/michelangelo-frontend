#pragma once

#include <string>
#include <vector>

namespace MichelangeloAPI
{
	struct ObjectGeometry
	{
		std::string Name;
	};

	struct SceneGeometry
	{
		std::vector<ObjectGeometry> Objects;
	};
}