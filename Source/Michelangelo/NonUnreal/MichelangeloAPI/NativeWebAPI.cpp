#include "Michelangelo.h"
#include "NativeWebAPI.h"
#include "HeaderConstants.h"
#include "URLConstants.h"
#include "NonUnreal/nlohmann/JSON/json.hpp"
#include "NonUnreal/Common/EngineException.h"
#include "CameraParameters.h"
#include "CurlException.h"
#include "CurlHandle.h"
#include "CurlList.h"
#include "CurlBody.h"
#include "Exceptions/AuthenticationError.h"
#include "Exceptions/TimeoutError.h"

#include <regex>
#include <limits>

using namespace Common;
using namespace MichelangeloAPI;
using namespace std;

NativeWebAPI::NativeWebAPI()
{
	// Initialize windows sockets:
	CurlAssert(curl_global_init(CURL_GLOBAL_DEFAULT));
}
NativeWebAPI::~NativeWebAPI()
{
	curl_global_cleanup();
}

void NativeWebAPI::Authenticate(const std::string& email, const std::string& password, bool rememberMe)
{
	CurlHandle curlHandle;

	// Ensure that a new session is used:
	m_sessionData.Reset();

	std::string requestVerificationToken;
	// Extract request verification token and cookie value:
	{
		CurlList requestHeader;
		std::string responseHeader;
		std::string responseBody;
		curlHandle.PerformGETRequest(URLConstants::LogInAPI, requestHeader, responseHeader, responseBody);

		// Extract the request verification token:
		if (!ExtractLogInVerificationToken(responseBody, requestVerificationToken))
			ThrowEngineException(L"Couldn't extract request verification token.");

		// Extract the request verification token cookie value:
		std::string requestVerificationTokenCookieValue;
		if (!ExtractCookieValue(responseHeader, HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenCookieValue))
			ThrowEngineException(L"Couldn't extract request verification cookie value.");
		m_sessionData.AddCookie(HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenCookieValue);
	}

	// Perform a post request, sending the login information, and set the corresponding cookies:
	{
		auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
		CurlBody requestBody;
		requestBody.AddPair("__RequestVerificationToken", requestVerificationToken);
		requestBody.AddPair("Email", email);
		requestBody.AddPair("Password", password);
		requestBody.AddPair("RememberMe", rememberMe ? "true" : "false");
		requestBody.Generate(curlHandle.Get(), false);

		std::string responseHeader;
		std::string responseBody;
		curlHandle.PerformPOSTRequest(URLConstants::LogInAPI, requestHeader, requestBody, responseHeader, responseBody);

		// Extract the application cookie:
		std::string applicationCookieValue;
		if (!ExtractCookieValue(responseHeader, HeaderConstants::ApplicationCookieName, applicationCookieValue))
			throw AuthenticationError("Wrong Credentials!");
		m_sessionData.AddCookie(HeaderConstants::ApplicationCookieName, applicationCookieValue);
	}
}
void NativeWebAPI::LogOut()
{
	if (!m_sessionData.IsAuthenticated())
		return;

	CurlHandle curlHandle;

	// Extract the request verification token from the main page:
	std::string requestVerificationTokenValue;
	{
		auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
		std::string responseHeader;
		std::string responseBody;
		curlHandle.PerformGETRequest(URLConstants::MainPage, requestHeader, responseHeader, responseBody);

		// Extract the request verification token:
		if (!ExtractLogOutVerificationToken(responseBody, requestVerificationTokenValue))
			ThrowEngineException(L"Couldn't extract verification token.");
	}

	// Perform logout request:
	{
		auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
		CurlBody requestBody;
		requestBody.AddPair(HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenValue);
		requestBody.Generate(curlHandle.Get(), true);

		std::string responseHeader;
		std::string responseBody;
		curlHandle.PerformPOSTRequest(URLConstants::LogOutAPI, requestHeader, requestBody, responseHeader, responseBody);
	}

	m_sessionData.Reset();
}

GrammarSpecificData NativeWebAPI::CreateNewGrammar() const
{
	CurlHandle curlHandle;

	auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
	CurlBody requestBody;
	std::string responseHeader, responseBody;
	curlHandle.PerformPUTRequest(URLConstants::OwnGrammarAPI, requestHeader, requestBody, responseHeader, responseBody);

	return GrammarSpecificData::FromJson(nlohmann::json::parse(responseBody.c_str()), GrammarType::Own);
}
void NativeWebAPI::DeleteGrammar(const std::string& id) const
{
	CurlHandle curlHandle;

	auto url = URLConstants::OwnGrammarAPI + id;
	auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
	std::string responseHeader, responseBody;
	curlHandle.PerformDELETERequest(url, requestHeader, responseHeader, responseBody);
}
void NativeWebAPI::ShareGrammar(const std::string& id, bool share) const
{
	CurlHandle curlHandle;

	auto url = URLConstants::OwnGrammarAPI + "share/" + (share ? "true" : "false") + "/" + id;
	auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
	CurlBody requestBody;
	{
		requestBody.AddPair("share", (share ? "true" : "false"));
		requestBody.Generate(curlHandle.Get(), true);
	}
	std::string responseHeader, responseBody;
	curlHandle.PerformPUTRequest(url, requestHeader, requestBody, responseHeader, responseBody);
}

std::vector<GrammarSpecificData> NativeWebAPI::GetGrammarsList(GrammarType grammarType) const
{
	// Perform web request to fetch list of grammars:
	CurlHandle curlHandle;
	auto grammarsJson = PerformGetJsonRequest(curlHandle, URLConstants::GetGrammarsListURL.at(grammarType));

	std::vector<GrammarSpecificData> grammarsData;
	grammarsData.reserve(grammarsJson.size());
	for (const auto& element : grammarsJson)
		grammarsData.push_back(GrammarSpecificData::FromJson(element, grammarType));

	return grammarsData;
}

GrammarSpecificData NativeWebAPI::GetGrammarData(GrammarType grammarType, const std::string& grammarID) const
{
	CurlHandle curlHandle;
	auto grammarsJson = PerformGetJsonRequest(curlHandle, URLConstants::GetGrammarDataURL.at(grammarType) + grammarID);
	return GrammarSpecificData::FromJson(grammarsJson, grammarType);
}

void NativeWebAPI::EvaluateGrammar(const GrammarSpecificData& grammarData, const CameraParameters& cameraParameters, SceneGeometry& sceneGeometry, std::string& errorMessage) const
{
	CurlHandle curlHandle;

	// Perform request:
	std::string responseBody;
	{
		CurlBody requestBody;
		requestBody.AddPair("ID", grammarData.ID);
		requestBody.AddPair("Name", grammarData.Name);
		requestBody.AddPair("Type", grammarData.Type);
		requestBody.AddPair("Code", Helpers::WStringToString(grammarData.Code));
		requestBody.AddPair("OnlyNID", std::to_string(std::numeric_limits<unsigned int>::max()));
		requestBody.AddPair("CamPos", Helpers::ArrayToString(cameraParameters.GetCameraPosition()));
		requestBody.AddPair("CamUp", Helpers::ArrayToString(cameraParameters.GetCameraUpDirection()));
		requestBody.AddPair("CamLook", Helpers::ArrayToString(cameraParameters.GetCameraUpDirection()));
		requestBody.AddPair("CamFOV", std::to_string(cameraParameters.GetCameraFov()));
		requestBody.AddPair("Width", std::to_string(cameraParameters.GetWidth()));
		requestBody.AddPair("Height", std::to_string(cameraParameters.GetHeight()));
		requestBody.Generate(curlHandle.Get(), true);

		auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
		std::string responseHeader;
		curlHandle.PerformPOSTRequest(URLConstants::EvaluateGrammarURL.at(grammarData.GrammarType), requestHeader, requestBody, responseHeader, responseBody);
	}

	// Parse response json:
	auto dataJson = nlohmann::json::parse(responseBody.c_str());

	// Handle long requests:
	{
		auto location = dataJson.find("img");
		if (location != dataJson.end())
		{
			auto image = location->get<std::string>();
			if (image.size() > 0)
			{
				auto parsedJson = dataJson.at("parsedJSON");
				if (image == parsedJson)
				{
					// Wait for long task to finish:
					dataJson = WaitForLongTaskToFinish(curlHandle, grammarData, dataJson);
				}
			}
		}
	}

	// Find if any unexpected error occurred:
	{
		auto location = dataJson.find("message");
		if (location != dataJson.end())
		{
			errorMessage = location->get<std::string>();
			return;
		}
	}

	// Find if gramatical errors occurred (including warnings):
	errorMessage = dataJson.at("e").get<std::string>();

	// Create geometry from json:
	sceneGeometry = SceneGeometry::CreateFromJson(dataJson);
}

const SessionData& NativeWebAPI::GetSessionData() const
{
	return m_sessionData;
}

nlohmann::json NativeWebAPI::WaitForLongTaskToFinish(const CurlHandle& curlHandle, const GrammarSpecificData& grammarData, const nlohmann::json& responseData) const
{
	auto key = responseData.at("img").get<std::string>();
	auto url = URLConstants::EvaluateGrammarURL.at(grammarData.GrammarType) + grammarData.ID + "/Response/" + key;

	// Try for at most 1 hour:
	auto startTime = chrono::high_resolution_clock::now();
	auto endTime = startTime + chrono::hours(1);
	while (startTime < endTime)
	{
		auto dataJson = PerformGetJsonRequest(curlHandle, url);

		auto location = dataJson.find("img");
		if (location == dataJson.end())
			ThrowEngineException(L"Expected parameter 'img' in JSON.");

		auto image = location->get<std::string>();
		if (image != key)
			return dataJson;
	}

	throw TimeoutError("Long request timeout.");
}

bool NativeWebAPI::ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookieValue)
{
	// Check if there's a set-cookie in the response:
	if (header.find(HeaderConstants::SetCookie) == std::string::npos)
		return false;

	// Find the cookie name start:
	auto cookieNameStart = header.find(cookieName);
	if (cookieNameStart == std::string::npos)
		return false;

	// Calculate the start of the cookie value:
	auto cookieValueStart = cookieNameStart + cookieName.size() + 1;

	// Find the end of the cookie value:
	auto cookieValueEnd = header.find(HeaderConstants::Semicolon, cookieNameStart);

	// Extract the cookie value:
	cookieValue = header.substr(cookieValueStart, cookieValueEnd - cookieValueStart);

	return true;
}
bool NativeWebAPI::ExtractLogInVerificationToken(const std::string& body, std::string& verificationToken)
{
	// TODO now it works only for direct registrations on the site, it would be good to extend it to external logins (Google, Facebook) if possible. They have a different token and possibly the login sequence is different as well.
	std::regex tokenRegex("form action=\"\\/Account\\/Login\" .*><input name=\"__RequestVerificationToken\" type=\"hidden\" value=\".*\"");
	std::smatch match;
	std::string tag;
	if (!std::regex_search(body, match, tokenRegex))
		return false;

	for (auto x : match) tag = x;

	// Now there should be quite a large substring in the tag including the form and its first input field (which bears the token)
	std::regex tokenValue_regex("value=\".*\""); // Match the value of the token
	if (!std::regex_search(tag, match, tokenValue_regex))
		return false;

	for (auto x : match) tag = x;

	verificationToken = tag.substr(7, tag.length() - 8);

	return true;
}
bool NativeWebAPI::ExtractLogOutVerificationToken(const std::string& body, std::string& verificationToken)
{
	std::regex tokenRegex("form action=\"\\/Account\\/LogOff\" .*><input name=\"__RequestVerificationToken\" type=\"hidden\" value=\".*\"");
	std::smatch match;
	std::string tag;
	if (!std::regex_search(body, match, tokenRegex))
		return false;

	for (auto x : match) tag = x;

	// Now there should be quite a large substring in the tag including the form and its first input field (which bears the token)
	std::regex tokenValue_regex("value=\".*\""); // Match the value of the token
	if (!std::regex_search(tag, match, tokenValue_regex))
		return false;

	for (auto x : match) tag = x;

	verificationToken = tag.substr(7, tag.length() - 8);

	return true;
}

nlohmann::json NativeWebAPI::PerformGetJsonRequest(const CurlHandle& curlHandle, const std::string& url) const
{
	auto requestHeader = m_sessionData.GenerateHeaderWithCookies();
	std::string responseHeader, responseBody;
	curlHandle.PerformGETRequest(url, requestHeader, responseHeader, responseBody);

	// Create json object:
	return nlohmann::json::parse(responseBody.c_str());
}