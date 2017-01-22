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
		ObjectGeometry() = default;
		virtual ~ObjectGeometry() = default;

		Type GetType() const;
		const std::string& GetName() const;
		const std::vector<float>& GetVertices() const;
		const std::vector<float>& GetIndices() const;
		const std::array<float, 16>& GetTransform() const;

		static ObjectGeometry CreateFromJSON(const nlohmann::json& jsonObject);

	private:
		Type m_type = Type::StaticMesh;
		std::string m_name;
		std::vector<float> m_vertices;
		std::vector<float> m_indices;
		std::array<float, 16> m_transform;
	};
}
