#pragma once

#include <array>
#include <sstream>
#include <string>
#include <unordered_map>

namespace TextHighlight
{
	class BaseTextPiece
	{
	public:
		virtual ~BaseTextPiece()
		{
		}

		BaseTextPiece() = default;
		
		virtual std::string ToString() const = 0;

	protected:
		static std::string EarlyEscapeText(const std::string& text)
		{
			std::unordered_map<char, std::string> escapeCharacters =
			{
				{ '&', "&amp;" },
				{ '<', "&lt;" },
				{ '>', "&gt;" }
			};

			std::stringstream ss;
			for (auto iterator = text.begin(); iterator != text.end(); ++iterator)
			{
				auto pair = escapeCharacters.find(*iterator);
				if (pair != escapeCharacters.end())
				{
					ss << pair->second;
				}
				else
				{
					ss << *iterator;
				}
			}

			return ss.str();
		}

		static std::string EscapeText(const std::string& text, bool escapeNewLines = false)
		{
			std::unordered_map<char, std::string> escapeCharacters =
			{
				{ '"', "&quot;" },
			};
			if (escapeNewLines)
				escapeCharacters.emplace('\n', "\\n");

			std::stringstream ss;
			for(auto iterator = text.begin(); iterator != text.end(); ++iterator)
			{
				auto pair = escapeCharacters.find(*iterator);
				if(pair != escapeCharacters.end())
				{
					ss << pair->second;
				}
				else
				{
					ss << *iterator;
				}
			}

			return ss.str();
		}
	};
}
