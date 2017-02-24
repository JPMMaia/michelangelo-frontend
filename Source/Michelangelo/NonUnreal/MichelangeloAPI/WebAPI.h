#pragma once

#include "NonUnreal/MichelangeloAPI/GrammarData.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"
#include "NonUnreal/MichelangeloAPI/CurlList.h"
#include "NonUnreal/MichelangeloAPI/CurlPost.h"
#include "NonUnreal/nlohmann/JSON/json.hpp"
#include "IncludeCurl.h"

#include <initializer_list>
#include <unordered_map>

namespace MichelangeloAPI
{
	class CameraParameters;

	class WebAPI
	{
	public:
		enum class LoginError
		{
			None,
			WrongCredentials,
			Unknown
		};

	public:
		WebAPI();
		~WebAPI();

		LoginError Authenticate(const std::string& email, const std::string& password, bool rememberMe);
		void LogOut();

		std::vector<GrammarData> GetGrammars(const std::string& url) const;
		GrammarSpecificData GetGrammarSpecificData(const std::string& url, const std::string& grammarID) const;
		bool GetGeometry(const std::string& url, const GrammarSpecificData& data, const CameraParameters& cameraParameters, SceneGeometry& sceneGeometry, std::string& errorMessage) const;

		CURL* GetCURL();
		const CURL* GetCURL() const;

		bool IsAuthenticated() const;

	private:
		void Initialize();
		void Shutdown();

		bool PerformGETRequest(const std::string& url, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		bool PerformPOSTRequest(const std::string& url, CurlList& requestHeader, const CurlPost& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		nlohmann::json PerformGETJSONRequest(const std::string& url) const;

		void AddCookie(const std::string& name, const std::string& value);
		void SetCookie(CurlList& header) const;
		
		static int WriteCallback(char* data, size_t size, size_t count, std::string* userData);
		static bool ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookieValue);
		static bool ExtractLogInVerificationToken(const std::string& body, std::string& verificationToken);
		static bool ExtractLogOutVerificationToken(const std::string& body, std::string& verificationToken);

	private:
		CURL* m_curl = nullptr;
		
		std::unordered_map<std::string, std::string> m_cookies;
		std::string m_cookieString;
		bool m_isAuthenticated = false;
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
