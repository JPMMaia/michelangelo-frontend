#pragma once

#include "NonUnreal/nlohmann/JSON/json.hpp"
#include "NonUnreal/Common/Helpers.h"

namespace MichelangeloAPI
{
	struct GrammarSpecificData
	{
		std::string ID;
		std::string Name;
		std::string Type;
		std::wstring Code;
		bool Shared;
		bool IsOwner;

		static GrammarSpecificData FromJson(const nlohmann::json& objectJson)
		{
			GrammarSpecificData object;
			object.ID = objectJson.at("id").get<std::string>();
			object.Name = objectJson.at("name").get<std::string>();
			object.Type = objectJson.at("type").get<std::string>();
			object.Code = Common::Helpers::StringToWString(objectJson.at("code").get<std::string>());
			object.Shared = objectJson.at("shared").get<bool>();
			object.IsOwner = objectJson.at("isOwner").get<bool>();
			return object;
		}
	};
}
