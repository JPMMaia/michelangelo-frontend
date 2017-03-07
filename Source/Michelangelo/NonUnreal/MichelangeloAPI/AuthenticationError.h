#pragma once

namespace MichelangeloAPI
{
	class AuthenticationError : public std::exception
	{
	public:
		AuthenticationError() = default;
		explicit AuthenticationError(const std::string& message) :
			std::exception(message.c_str())
		{
		}
	};
}
