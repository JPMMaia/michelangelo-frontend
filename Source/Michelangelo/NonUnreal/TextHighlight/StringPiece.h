#pragma once

#include "BaseTextPiece.hpp"

namespace TextHighlight
{
	class StringPiece : public BaseTextPiece
	{
	public:
		StringPiece() = default;
		explicit StringPiece(const std::string& text) :
			m_text(text)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "<span color=\"#D4713F\">" << m_text << "</>";
			return ss.str();
		}

	private:
		std::string m_text;
	};
}
