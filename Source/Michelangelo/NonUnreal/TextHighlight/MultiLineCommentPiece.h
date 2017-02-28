#pragma once

#include "BaseTextPiece.hpp"
#include "CSharpHighlightSettings.hpp"

namespace TextHighlight
{
	class MultiLineCommentPiece : public BaseTextPiece
	{
	public:
		MultiLineCommentPiece() = default;
		explicit MultiLineCommentPiece(const std::string& text) :
			m_text(text)
		{
		}

		std::string ToString() const override
		{
			auto color = CSharpHighlightSettings::Get()->GetColor("comments");

			std::stringstream ss;

			auto escapedText = EscapeText(m_text);
			auto iterator = escapedText.begin();

			while (iterator != escapedText.end())
			{
				auto endLineLocation = std::find(iterator, escapedText.end(), '\n');

				ss << "<span color=\"#" + color + "\">";
				while (iterator != endLineLocation)
					ss << *iterator++;
				ss << "</>";

				if(iterator != escapedText.end())
				{
					ss << std::endl;
					++iterator;
				}
			}

			return ss.str();
		}

	private:
		std::string m_text;
	};
}
