#pragma once

#include "BaseTextPiece.hpp"

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
			std::stringstream ss;

			auto iterator = m_text.begin();

			while (iterator != m_text.end())
			{
				auto endLineLocation = std::find(iterator, m_text.end(), '\n');

				ss << "<span color=\"#48A433\">";
				while (iterator != endLineLocation)
					ss << *iterator++;
				ss << "</>\n";

				++iterator;
			}

			return ss.str();
		}

	private:
		std::string m_text;
	};
}
