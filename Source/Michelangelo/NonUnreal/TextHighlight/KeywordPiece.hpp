#pragma once

#include "BaseTextPiece.hpp"

namespace TextHighlight
{
	class KeywordPiece : public BaseTextPiece
	{
	public:
		KeywordPiece() = default;
		explicit KeywordPiece(const std::string& text) :
			m_text(text)
		{
		}

		std::string ToString() const override
		{
			auto color = CSharpHighlightSettings::Get()->GetColor("keywords");

			std::stringstream ss;
			ss << "<span color=\"#" + color + "\">" << EscapeText(m_text) << "</>";
			return ss.str();
		}

	private:
		std::string m_text;
	};
}
