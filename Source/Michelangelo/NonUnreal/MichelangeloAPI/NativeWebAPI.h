#pragma once

#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"
#include "NonUnreal/nlohmann/JSON/json.hpp"

#include "SessionData.h"

namespace MichelangeloAPI
{
	class CurlHandle;
	class CameraParameters;

	class NativeWebAPI
	{
	public:
		NativeWebAPI();
		~NativeWebAPI();

		void Authenticate(const std::string& email, const std::string& password, bool rememberMe);
		void LogOut();

		GrammarSpecificData CreateNewGrammar() const;
		void DeleteGrammar(const std::string& id) const;
		void ShareGrammar(const std::string& id, bool share) const;

		std::vector<GrammarSpecificData> GetGrammars(const std::string& url) const;
		GrammarSpecificData GetGrammarSpecificData(const std::string& url, const std::string& grammarID) const;
		bool GetGeometry(const std::string& url, const GrammarSpecificData& data, const CameraParameters& cameraParameters, SceneGeometry& sceneGeometry, std::string& errorMessage) const;

		const SessionData& GetSessionData() const;

	private:
		static bool ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookieValue);
		static bool ExtractLogInVerificationToken(const std::string& body, std::string& verificationToken);
		static bool ExtractLogOutVerificationToken(const std::string& body, std::string& verificationToken);

	private:
		nlohmann::json NativeWebAPI::PerformGetJsonRequest(const CurlHandle& curlHandle, const std::string& url) const;

	private:
		SessionData m_sessionData;
	};
}
