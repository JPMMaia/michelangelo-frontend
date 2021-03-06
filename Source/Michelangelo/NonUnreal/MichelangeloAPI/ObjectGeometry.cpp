#include "Michelangelo.h"
#include "ObjectGeometry.h"

#include <algorithm>
#include "Unreal/Common/UnrealHelpers.h"

using namespace MichelangeloAPI;

ObjectGeometry ObjectGeometry::CreateFromJSON(const nlohmann::json& jsonObject)
{
	ObjectGeometry geometry;

	auto name = jsonObject.at("g").get<std::string>();
	geometry.m_name = name;

	// Parse the material index:
	{
		geometry.m_materialIndex = jsonObject.at("m").get<size_t>();
	}

	// Parse the transformation matrix:
	{
		const auto& transformArray = jsonObject.at("t");
		std::copy(transformArray.cbegin(), transformArray.cend(), geometry.m_transform.begin());
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
		geometry.m_type = Type::ProceduralMesh;

		auto verticesJSON = jsonObject.at("v");

		// Parse coordinates:
		const auto& coordinates = verticesJSON.at("points");
		geometry.m_vertices.resize(coordinates.size());
		std::copy(coordinates.cbegin(), coordinates.cend(), geometry.m_vertices.begin());
		
		// Transform to unreal engine coordinates:
		std::transform(geometry.m_vertices.begin(), geometry.m_vertices.end(), geometry.m_vertices.begin(),
			[](float value) { return value * 100.0f; });

		{
			auto iterator = geometry.m_vertices.begin();
			while (iterator != geometry.m_vertices.end())
			{
				auto& y = *iterator++;
				auto& x = *iterator++;
				auto& z = *iterator++;
				FVector vertex(x, y, z);

				auto transform = Common::Helpers::ArrayToMatrix(geometry.m_transform);
				transform = Common::Helpers::MichelangeloToUnrealGeneralTransform(transform);
				vertex = transform.TransformPosition(vertex);
				
				x = vertex.X;
				y = vertex.Y;
				z = vertex.Z;
			}
		}

		// If the mesh is indexed:
		if (verticesJSON.at("indexed").get<bool>())
		{
			// Parse indices:
			const auto& indicesArray = verticesJSON.at("indices");
			geometry.m_indices.resize(indicesArray.size());
			std::copy(indicesArray.cbegin(), indicesArray.cend(), geometry.m_indices.begin());
		}

		// If the mesh is not indexed:
		else
		{
			// Generate indices:
			geometry.m_indices.resize(geometry.m_vertices.size());
			std::iota(geometry.m_indices.begin(), geometry.m_indices.end(), 0);
		}

		return geometry;
	}

	// Camera:
	else if (name == "camera")
	{
		geometry.m_type = Type::Camera;
	}

	// Static:
	else if (name == "box" || name == "cone" || name == "cylinder" || name == "sphere")
	{
		geometry.m_type = Type::StaticMesh;
	}

	// Unknown:
	else
	{
		geometry.m_type = Type::Unknown;
	}

	return geometry;
}


ObjectGeometry::Type ObjectGeometry::GetType() const
{
	return m_type;
}
const std::string& ObjectGeometry::GetName() const
{
	return m_name;
}
size_t ObjectGeometry::GetMaterialIndex() const
{
	return m_materialIndex;
}
const std::vector<float>& ObjectGeometry::GetVertices() const
{
	return m_vertices;
}
const std::vector<int32>& ObjectGeometry::GetIndices() const
{
	return m_indices;
}
const std::array<float, 16>& ObjectGeometry::GetTransform() const
{
	return m_transform;
}

bool ObjectGeometry::IsMesh() const
{
	return m_type == Type::StaticMesh || m_type == Type::ProceduralMesh;
}
