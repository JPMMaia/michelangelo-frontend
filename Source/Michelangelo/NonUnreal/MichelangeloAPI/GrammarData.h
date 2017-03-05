#pragma once

#include <string>
#include "NonUnreal/nlohmann/JSON/json.hpp"

namespace MichelangeloAPI
{
	struct GrammarData
	{
		std::string ID;
		std::string Name;
		std::string Type;

		static GrammarData FromJson(const nlohmann::json& objectJson)
		{
			GrammarData object;
			object.ID = objectJson.at("id").get<std::string>();
			object.Name = objectJson.at("name").get<std::string>();
			object.Type = objectJson.at("type").get<std::string>();
			return object;
		}
	};
}