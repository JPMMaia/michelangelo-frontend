#pragma once

#include "NonUnreal/Common/Helpers.h"
#include "NonUnreal/Common/EngineException.h"

#include <cassert>

#ifndef ThrowIfCURLFailed
#define ThrowIfCURLFailed(curlCode)													\
{																					\
	if(curlCode != CURLE_OK)														\
	{																				\
		auto message = L"CURL error: " + Common::Helpers::StringToWString(std::string(curl_easy_strerror(curlCode))); \
		std::wstring functionName = Common::Helpers::StringToWString(__FUNCSIG__);	\
		std::wstring filename = Common::Helpers::StringToWString(__FILE__);			\
		throw Common::EngineException(message, functionName, filename, __LINE__);			\
	}																				\
}
#endif

#ifndef CurlAssert
#define CurlAssert(functionCall)	\
{									\
	auto curlCode = functionCall;	\
	assert(curlCode == CURLE_OK);	\
}

#endif