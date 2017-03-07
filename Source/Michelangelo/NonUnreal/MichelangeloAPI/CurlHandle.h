#pragma once

#include "IncludeCurl.h"
#include "NonUnreal/nlohmann/JSON/json.hpp"

#include <string>

namespace MichelangeloAPI
{
	class SessionData;
	class CurlList;
	class CurlBody;

	class CurlHandle
	{
	public:
		CurlHandle();
		~CurlHandle();

		void PerformGETRequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const;
		void PerformPOSTRequest(const std::string& url, const CurlList& requestHeader, const CurlBody& requestBody, std::string& responseHeader, std::string& responseBody) const;
		void PerformPUTRequest(const std::string& url, const CurlList& requestHeader, const CurlBody& requestBody, std::string& responseHeader, std::string& responseBody) const;
		void PerformDELETERequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const;

		CURL* Get() const;

	private:
		static int WriteCallback(char* data, size_t size, size_t count, std::string* userData);

	private:
		void InitializeRequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const;

	private:
		CURL* m_handle = nullptr;
	};
}
