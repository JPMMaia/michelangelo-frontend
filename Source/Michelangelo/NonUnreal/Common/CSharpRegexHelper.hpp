#pragma once

#include "NonUnreal/nlohmann/JSON/json.hpp"

#include <fstream>
#include <unordered_set>
#include <regex>

class CSharpRegexHelper
{
public:
	CSharpRegexHelper() = default;

	explicit CSharpRegexHelper(const std::wstring& jsonFilename)
	{
		using namespace std;

		ifstream fileStream(jsonFilename, ios::in);
		if (!fileStream.good())
			throw runtime_error("File not found!");

		nlohmann::json highlightingJson;
		fileStream >> highlightingJson;

		auto keywordsArray = highlightingJson.at("keywords");
		for (const auto& keyword : keywordsArray)
		{
			m_keywords.insert(keyword.get<std::string>());
		}

		BuildRegex();
	}

	const std::regex& GetKeywordsRegex() const
	{
		return m_keywordsRegex;
	}

private:

	void BuildRegex()
	{
		using namespace std;

		// Keywords:
		{
			std::stringstream keywordsRegexSS;

			keywordsRegexSS << "\\b(";

			auto keywordCount = m_keywords.size();
			for(const auto& keyword : m_keywords)
			{
				keywordsRegexSS << keyword;
				if (--keywordCount != 0)
					keywordsRegexSS << "|";
			}

			keywordsRegexSS << ")\\b";

			m_keywordsRegex = std::regex(keywordsRegexSS.str());
		}
	}

private:
	std::unordered_set<std::string> m_keywords;
	std::regex m_keywordsRegex;
};
