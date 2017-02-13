#pragma once

#include "IncludeCurl.h"

#include <string>
#include <unordered_map>

namespace MichelangeloAPI
{
	class CurlPost
	{
	public:
		void AddPair(const std::string& name, const std::string& value);
		void Generate(CURL* curl, bool urlEncode);

		const std::string& GetData() const;

	private:
		std::unordered_map<std::string, std::string> m_pairs;
		std::string m_data;
	};
}
