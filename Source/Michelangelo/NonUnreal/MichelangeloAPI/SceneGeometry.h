#pragma once

#include "ObjectGeometry.h"

#include <vector>

namespace MichelangeloAPI
{
	class SceneGeometry
	{
	public:
		static SceneGeometry CreateFromJson(const nlohmann::json& objectJson);

	public:
		const std::vector<ObjectGeometry>& GetObjects() const;

	private:
		std::vector<ObjectGeometry> m_objects;
	};
}
