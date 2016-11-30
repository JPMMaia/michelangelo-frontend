#include "Michelangelo.h"
#include "ObjectGeometry.h"

#include <algorithm>

using namespace MichelangeloAPI;

ObjectGeometry::Type ObjectGeometry::GetType() const
{
	return m_type;
}
const std::string& ObjectGeometry::GetName() const
{
	return m_name;
}
const std::vector<float>& ObjectGeometry::GetVertices() const
{
	return m_vertices;
}
const std::vector<float>& ObjectGeometry::GetIndices() const
{
	return m_indices;
}
const std::array<float, 16>& ObjectGeometry::GetTransform() const
{
	return m_transform;
}

ObjectGeometry ObjectGeometry::CreateFromJSON(const nlohmann::json jsonObject)
{
	ObjectGeometry geometry;

	auto name = jsonObject.at("g").get<std::string>();
	geometry.m_name = name;

	// Parse the transformation matrix:
	{
		auto transformArray = jsonObject.at("t");
		for (size_t i = 0; i < transformArray.size(); ++i)
		{
			geometry.m_transform[i] = transformArray[i];
		}
	}

	// Convert name to lower case:
	std::transform(geometry.m_name.begin(), geometry.m_name.end(), name.begin(), ::tolower);

	// Empty node:
	if (name == "empty")
	{
		geometry.m_type = Type::Empty;
	}
		
	// Mesh:
	else if (name == "mesh")
	{
		auto verticesJSON = jsonObject.at("v");
		
		// TODO decide if the mesh is indexed or not
		// TODO parse vertices and indices

		return geometry;
	}

	// Static:
	else
	{
		geometry.m_type = Type::Static;
	}

	return geometry;
}
