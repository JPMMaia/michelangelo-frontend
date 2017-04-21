#include "Michelangelo.h"
#include "SceneGeometry.h"

using namespace MichelangeloAPI;

SceneGeometry SceneGeometry::CreateFromJson(const nlohmann::json& sceneGeometryJson)
{
	SceneGeometry scene;

	// Materials:
	{
		const auto& materialsJson = sceneGeometryJson.at("ml");
		scene.m_materials.reserve(materialsJson.size());
		for (auto iterator = materialsJson.cbegin(); iterator != materialsJson.cend(); ++iterator)
		{
			auto materialStr = iterator.value().get<std::string>();
			auto materialJson = nlohmann::json::parse(materialStr.c_str());
			scene.m_materials.emplace(iterator.key(), Material::CreateFromJSON(materialJson));
		}
	}

	// Objects:
	{
		const auto& objectsJson = sceneGeometryJson.at("o");
		scene.m_objects.reserve(objectsJson.size());
		for (const auto& element : objectsJson)
		{
			scene.m_objects.push_back(ObjectGeometry::CreateFromJSON(element));
		}
	}

	return scene;
}

const std::unordered_map<std::string, Material>& SceneGeometry::GetMaterials() const
{
	return m_materials;
}
const std::vector<ObjectGeometry>& SceneGeometry::GetObjects() const
{
	return m_objects;
}

bool SceneGeometry::IsEmpty() const
{
	return m_objects.empty();
}
