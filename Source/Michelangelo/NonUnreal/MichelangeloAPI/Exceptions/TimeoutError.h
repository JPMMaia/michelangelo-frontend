#pragma once

#include <string>

namespace MichelangeloAPI
{
	class TimeoutError : public std::exception
	{
	public:
		TimeoutError() = default;
		explicit TimeoutError(const std::string& message) :
			std::exception(message.c_str())
		{
		}
	};
}
