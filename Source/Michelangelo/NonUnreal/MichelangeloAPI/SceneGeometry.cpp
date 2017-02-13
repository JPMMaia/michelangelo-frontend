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
		for (auto& element : materialsJson)
		{
			auto str = element.get<std::string>();
			auto jsonValue = nlohmann::json::parse(str.c_str());
			scene.m_materials.push_back(Material::CreateFromJSON(jsonValue));
		}
	}

	// Objects:
	{
		const auto& objectsJson = sceneGeometryJson.at("o");
		scene.m_objects.reserve(objectsJson.size());
		for (auto& element : objectsJson)
		{
			scene.m_objects.push_back(ObjectGeometry::CreateFromJSON(element));
		}
	}

	return scene;
}

const std::vector<Material>& SceneGeometry::GetMaterials() const
{
	return m_materials;
}
const std::vector<ObjectGeometry>& SceneGeometry::GetObjects() const
{
	return m_objects;
}
