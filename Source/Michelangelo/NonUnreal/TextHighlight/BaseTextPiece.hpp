#pragma once

#include <array>
#include <sstream>
#include <string>

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
	};
}
