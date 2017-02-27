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
			std::stringstream ss;
			ss << "<span color=\"#4A9A9B\">" << m_text << "</>";
			return ss.str();
		}

	private:
		std::string m_text;
	};
}
