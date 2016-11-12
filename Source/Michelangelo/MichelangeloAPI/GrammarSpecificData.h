#pragma once

#include <string>

namespace MichelangeloAPI
{
	struct GrammarSpecificData
	{
		std::string ID;
		std::string Name;
		std::string Type;
		std::string Code;
		bool Shared;
		bool IsOwner;
	};
}