#pragma once

#include "BaseTextPiece.hpp"

namespace TextHighlight
{
	class OneLineCommentPiece : public BaseTextPiece
	{
	public:
		OneLineCommentPiece() = default;

		explicit OneLineCommentPiece(const std::string& text) :
			m_text(text)
		{
		}

		std::string ToString() const override
		{
			auto color = CSharpHighlightSettings::Get()->GetColor("comments");

			std::stringstream ss;
			ss << "<span color=\"#" + color + "\">" << EscapeText(m_text) << "</>";
			return ss.str();
		}

	private:
		std::string m_text;
	};
}
