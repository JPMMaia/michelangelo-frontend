#pragma once

#include <exception>
#include <string>

namespace MichelangeloAPI
{
	class BaseError : public std::exception
	{
	public:
		BaseError() = default;
		explicit BaseError(const std::string& message) :
			std::exception(message.c_str())
		{
		}
	};
}
