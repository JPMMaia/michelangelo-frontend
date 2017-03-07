#pragma once

#include "Helpers.h"

#include <exception>

namespace Common
{
	class EngineException : public std::exception
	{
	public:
		EngineException() = default;
		EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	public:
		char const* what() const override;

	private:
		std::string m_message;
	};

#ifndef ThrowEngineException
#define ThrowEngineException(message)											\
{																				\
	auto __functionName__ = Common::Helpers::StringToWString(__FUNCSIG__);	\
	auto __filename__ = Common::Helpers::StringToWString(__FILE__);			\
	throw Common::EngineException(message, __functionName__, __filename__, __LINE__);			\
}
#endif

}
