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

			// Get highlighing json:
			nlohmann::json highlightingJson;
			{
				// Try to open file:
				ifstream fileStreamIn(jsonFilename, ios::in);

				// If it exists, then read content:
				if (fileStreamIn.good())
				{
					fileStreamIn >> highlightingJson;
				}

				// If not:
				else
				{
					// Create default json:
					highlightingJson = DefaultHighlighintJson();

					// Output json to file:
					ofstream fileStreamOut(jsonFilename, ios::out);
					fileStreamOut << highlightingJson;
				}
			}

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
				for (auto iterator = colorsJson.begin(); iterator != colorsJson.end(); ++iterator)
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

		static nlohmann::json DefaultHighlighintJson()
		{
			return {
				{ "keywords", {
					"abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue","decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for",
				"foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator",
				"out", "override", "params", "private", "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static",
				"string", "struct", "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void",
				"volatile", "while", "add", "alias", "ascending", "async", "await", "descending", "dynamic", "from", "get", "global", "group", "into", "join", "let", "orderby",
				"partial", "partial", "remove", "select", "set", "var", "where", "yield"
					}
				},
				{ "colors", {
					{ "comments", "51B53D"},
					{ "keywords", "4A9AE4"},
					{ "strings", "E27944" }
					}
				}
			};
		}

	private:
		std::unordered_set<std::string> m_keywords;
		std::regex m_keywordsRegex;
		std::unordered_map<std::string, std::string> m_colors;
	};
}
