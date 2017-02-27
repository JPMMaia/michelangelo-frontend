#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <unordered_set>

#include "NonUnreal/nlohmann/JSON/json.hpp"

namespace TextHighlight
{
	class CSharpCodeHighlighter
	{
	private:
		static CSharpCodeHighlighter* s_instance;

	public:
		static CSharpCodeHighlighter* Get()
		{
			if (!s_instance)
				s_instance = new CSharpCodeHighlighter(L"Highlighting.json");

			return s_instance;
		}

	public:

		const std::regex& GetKeywordsRegex() const
		{
			return m_keywordsRegex;
		}

	private:
		explicit CSharpCodeHighlighter(const std::wstring& jsonFilename)
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

			// One-line comments:
			{
				//m_oneLineCommentsRegex = std::regex("\/\/.*");
			}

			// Multi-line comments:
			{
				//m_multiLineCommentsRegex = std::regex("\/\*+((([^\*])+)|([\*]+(?!\/)))[*]+\/");
			}
		}

	private:
		std::unordered_set<std::string> m_keywords;
		std::regex m_keywordsRegex;
		std::regex m_oneLineCommentsRegex;
		std::regex m_multiLineCommentsRegex;
	};

	CSharpCodeHighlighter* CSharpCodeHighlighter::s_instance;
}
