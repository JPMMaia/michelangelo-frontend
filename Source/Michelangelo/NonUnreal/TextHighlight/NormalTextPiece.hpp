#pragma once

#include "BaseTextPiece.hpp"

namespace TextHighlight
{
	class NormalTextPiece : public BaseTextPiece
	{
	public:
		NormalTextPiece() = default;
		explicit NormalTextPiece(const std::string& text) :
			m_text(text)
		{
		}

		std::string ToString() const override
		{
			return EscapeText(m_text);
		}

	private:
		std::string m_text;
	};
}
