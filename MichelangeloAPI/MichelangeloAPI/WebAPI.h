#pragma once

#include "Common/EngineException.h"

#include <curl/curl.h>
#include <initializer_list>

namespace MichelangeloAPI
{
	class WebAPI
	{
	public:
		typedef void (*SListCleanup) (curl_slist*);
		typedef std::unique_ptr<curl_slist, SListCleanup> SListHandle;

	public:
		WebAPI();
		~WebAPI();

		void Authenticate();

		CURL* GetCURL();
		const CURL* GetCURL() const;

		curl_slist* GetCookie();
		const curl_slist* GetCookie() const;

	private:
		void Initialize();
		void Shutdown();

		static bool ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookie);
		static std::string BuildCookie(const std::initializer_list<std::string>& cookieValues);

	private:
		CURL* m_curl = nullptr;
		curl_slist* m_cookie = nullptr;
	};

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

}
