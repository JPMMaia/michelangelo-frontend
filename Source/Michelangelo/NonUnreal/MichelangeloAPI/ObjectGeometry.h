#pragma once

#include "NonUnreal/nlohmann/JSON/json.hpp"

#include <string>
#include <vector>

namespace MichelangeloAPI
{
	class ObjectGeometry
	{
	public:
		enum class Type
		{
			Empty,
			Camera,
			StaticMesh,
			ProceduralMesh,
			Unknown
		};

	public:
		static ObjectGeometry CreateFromJSON(const nlohmann::json& sceneGeometryJson);

	public:
		ObjectGeometry() = default;
		virtual ~ObjectGeometry() = default;

		Type GetType() const;
		const std::string& GetName() const;
		size_t GetMaterialIndex() const;
		const std::vector<float>& GetVertices() const;
		const std::vector<float>& GetIndices() const;
		const std::array<float, 16>& GetTransform() const;

	private:
		Type m_type = Type::StaticMesh;
		std::string m_name;
		size_t m_materialIndex = 0;
		std::vector<float> m_vertices;
		std::vector<float> m_indices;
		std::array<float, 16> m_transform;
	};
}
