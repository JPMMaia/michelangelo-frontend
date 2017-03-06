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

	class NativeWebAPI
	{
	public:
		enum class LoginError
		{
			None,
			WrongCredentials,
			Unknown
		};

	public:
		NativeWebAPI();
		~NativeWebAPI();

		LoginError Authenticate(const std::string& email, const std::string& password, bool rememberMe);
		void LogOut();

		GrammarSpecificData CreateNewGrammar() const;
		void DeleteGrammar(const std::string& id) const;
		void ShareGrammar(const std::string& id, bool share) const;

		std::vector<GrammarSpecificData> GetGrammars(const std::string& url) const;
		GrammarSpecificData GetGrammarSpecificData(const std::string& url, const std::string& grammarID) const;
		bool GetGeometry(const std::string& url, const GrammarSpecificData& data, const CameraParameters& cameraParameters, SceneGeometry& sceneGeometry, std::string& errorMessage) const;

		CURL* GetCURL();
		const CURL* GetCURL() const;

		bool IsAuthenticated() const;

	private:
		bool PerformGETRequest(const std::string& url, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		bool PerformPOSTRequest(const std::string& url, CurlList& requestHeader, const CurlPost& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		bool PerformPUTRequest(const std::string& url, CurlList& requestHeader, const CurlPost& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		bool PerformDELETERequest(const std::string& url, std::string& responseHeader, std::string& responseBody, bool setCookie) const;
		nlohmann::json PerformGETJSONRequest(const std::string& url) const;

		void AddCookie(const std::string& name, const std::string& value);
		void SetCookie(CurlList& header) const;
		
		static bool ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookieValue);
		static bool ExtractLogInVerificationToken(const std::string& body, std::string& verificationToken);
		static bool ExtractLogOutVerificationToken(const std::string& body, std::string& verificationToken);

	private:
		std::unordered_map<std::string, std::string> m_cookies;
		std::string m_cookieString;
		bool m_isAuthenticated = false;
	};
}
