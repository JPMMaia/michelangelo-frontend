#pragma once

#include "Material.h"
#include "ObjectGeometry.h"

#include <vector>

namespace MichelangeloAPI
{
	class SceneGeometry
	{
	public:
		static SceneGeometry CreateFromJson(const nlohmann::json& objectJson);

	public:
		const std::vector<Material>& GetMaterials() const;
		const std::vector<ObjectGeometry>& GetObjects() const;

	private:
		std::vector<Material> m_materials;
		std::vector<ObjectGeometry> m_objects;
	};
}
