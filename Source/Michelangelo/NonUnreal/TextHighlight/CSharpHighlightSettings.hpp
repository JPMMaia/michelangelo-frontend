#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>

#include "NonUnreal/nlohmann/JSON/json.hpp"

namespace TextHighlight
{
	class CSharpHighlightSettings
	{
	private:
		static CSharpHighlightSettings* s_instance;

	public:
		static CSharpHighlightSettings* Get()
		{
			if (!s_instance)
				s_instance = new CSharpHighlightSettings(L"Highlighting.json");

			return s_instance;
		}

	public:

		const std::regex& GetKeywordsRegex() const
		{
			return m_keywordsRegex;
		}

		const std::string& GetColor(const std::string& key) const
		{
			return m_colors.at(key);
		}


	private:
		explicit CSharpHighlightSettings(const std::wstring& jsonFilename)
		{
			using namespace std;

			ifstream fileStream(jsonFilename, ios::in);
			if (!fileStream.good())
				throw runtime_error("File not found!");

			nlohmann::json highlightingJson;
			fileStream >> highlightingJson;

			// Keywords:
			{
				auto keywordsArray = highlightingJson.at("keywords");
				for (const auto& keyword : keywordsArray)
				{
					m_keywords.insert(keyword.get<std::string>());
				}
				BuildRegex();
			}

			// Colors:
			{
				auto colorsJson = highlightingJson.at("colors");
				for(auto iterator = colorsJson.begin(); iterator != colorsJson.end(); ++iterator)
				{
					m_colors.emplace(iterator.key(), iterator.value().get<std::string>());
				}
			} 
		}

		void BuildRegex()
		{
			using namespace std;

			// Keywords:
			{
				std::stringstream keywordsRegexSS;

				keywordsRegexSS << "\\b(";

				auto keywordCount = m_keywords.size();
				for (const auto& keyword : m_keywords)
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
		std::unordered_map<std::string, std::string> m_colors;
	};
}
