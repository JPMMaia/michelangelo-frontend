#pragma once

#include "Material.h"
#include "ObjectGeometry.h"

#include <vector>
#include <unordered_map>

namespace MichelangeloAPI
{
	class SceneGeometry
	{
	public:
		static SceneGeometry CreateFromJson(const nlohmann::json& objectJson);

	public:
		const std::unordered_map<std::string, Material>& GetMaterials() const;
		const std::vector<ObjectGeometry>& GetObjects() const;

		bool IsEmpty() const;

	private:
		std::unordered_map<std::string, Material> m_materials;
		std::vector<ObjectGeometry> m_objects;
	};
}
