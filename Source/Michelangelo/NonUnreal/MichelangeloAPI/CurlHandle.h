#pragma once

#include "IncludeCurl.h"
#include "CurlList.h"
#include "Curl"

#include <string>

namespace MichelangeloAPI
{
	class CurlHandle
	{
	public:
		CurlHandle();
		~CurlHandle();

		void PerformGETRequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const;
		void PerformPOSTRequest(const std::string& url, CurlList& requestHeader, const CurlPost& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		void PerformPUTRequest(const std::string& url, CurlList& requestHeader, const CurlPost& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		void PerformDELETERequest(const std::string& url, std::string& responseHeader, std::string& responseBody, bool setCookie) const;

	private:
		static int WriteCallback(char* data, size_t size, size_t count, std::string* userData);

	private:
		CURL* m_handle = nullptr;
	};
}