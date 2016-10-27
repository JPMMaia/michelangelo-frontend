#pragma once

#include "Helpers.h"

namespace Common
{
	class EngineException
	{
	public:
		EngineException() = default;
		EngineException(const std::wstring& message, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

		std::wstring ToString() const;

	private:
		std::wstring m_message;
		std::wstring m_functionName;
		std::wstring m_filename;
		int m_lineNumber = -1;
	};

#ifndef ThrowEngineException
#define ThrowEngineException(message)											\
{																				\
	MessageBox(0, message, 0, 0);												\
	std::wstring functionName = Common::Helpers::StringToWString(__FUNCSIG__);	\
	std::wstring filename = Common::Helpers::StringToWString(__FILE__);			\
	throw EngineException(message, functionName, filename, __LINE__);			\
}
#endif

}
