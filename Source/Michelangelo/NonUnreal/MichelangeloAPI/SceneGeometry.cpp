#include "Michelangelo.h"
#include "SceneGeometry.h"

using namespace MichelangeloAPI;

SceneGeometry SceneGeometry::CreateFromJson(const nlohmann::json& sceneGeometryJson)
{
	SceneGeometry scene;

	const auto& objectsJson = sceneGeometryJson.at("o");
	scene.m_objects.reserve(objectsJson.size());
	for (auto& element : objectsJson)
	{
		scene.m_objects.push_back(ObjectGeometry::CreateFromJSON(element));
	}

	return scene;
}

const std::vector<ObjectGeometry>& SceneGeometry::GetObjects() const
{
	return m_objects;
}
