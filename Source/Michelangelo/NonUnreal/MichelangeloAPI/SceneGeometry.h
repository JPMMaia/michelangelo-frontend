#pragma once

#include <array>
#include <string>
#include <vector>
#include "ObjectGeometry.h"

namespace MichelangeloAPI
{
	struct SceneGeometry
	{
		std::vector<ObjectGeometry> Objects;
	};
}
