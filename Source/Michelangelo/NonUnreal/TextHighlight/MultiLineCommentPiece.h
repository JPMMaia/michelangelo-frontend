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

			auto iterator = m_text.begin();

			while (iterator != m_text.end())
			{
				auto endLineLocation = std::find(iterator, m_text.end(), '\n');

				ss << "<span color=\"#" + color + "\">";
				while (iterator != endLineLocation)
					ss << *iterator++;
				ss << "</>";

				if(iterator != m_text.end())
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
