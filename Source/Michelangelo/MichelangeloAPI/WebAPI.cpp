#include "Michelangelo.h"
#include "WebAPI.h"
#include "HeaderConstants.h"
#include "URLConstants.h"
#include "nlohmann/JSON/json.hpp"
#include "ObjectGeometry.h"

#include <CoreMisc.h>
#include <regex>

using namespace Common;
using namespace MichelangeloAPI;
using namespace std;

WebAPI::WebAPI()
{
	Initialize();
}
WebAPI::~WebAPI()
{
	Shutdown();
}

bool WebAPI::Authenticate(const std::string& username, const std::string& password, bool rememberMe)
{
	// TODO in case of timeout retry
	std::string loginPageBody;
	std::string loginPageHeader;
	if (!PerformGETRequest(URLConstants::LoginPage, loginPageHeader, loginPageBody, false))
		return false;

	// Extract the request verification token:
	std::string requestVerificationToken; 
	if (!ExtractVerificationToken(loginPageBody, requestVerificationToken))
		ThrowEngineException(L"Failed to extract verification token.");

	// Extract the request verification token cookie value:
	std::string requestVerificationTokenCookieValue;
	if (!ExtractCookieValue(loginPageHeader, HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenCookieValue))
		ThrowEngineException(L"Failed to extract verification cookie.");
	AddCookie(HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenCookieValue);

	// Perform a post request, sending the login information:
	std::string responseBody;
	std::string responseHeader;
	{
		auto postBody = "__RequestVerificationToken=" + requestVerificationToken + "&Email=" + username + "&Password=" + password + "&RememberMe=" + (rememberMe ? "true" : "false");
		CurlList requestHeader;
		if (!PerformPOSTRequest(URLConstants::LoginPage, requestHeader, postBody, responseHeader, responseBody, true))
			return false;
	}

	// Extract the application cookie:
	std::string applicationCookieValue; 
	if (!ExtractCookieValue(responseHeader, HeaderConstants::ApplicationCookieName, applicationCookieValue))
		ThrowEngineException(L"Failed to extract application cookie");
	AddCookie(HeaderConstants::ApplicationCookieName, applicationCookieValue);

	m_isAuthenticated = true;

	return true;
}

std::vector<GrammarData> WebAPI::GetGrammars(const std::string& url) const
{
	auto grammarJson = PerformGETJSONRequest(url);

	std::vector<GrammarData> grammarsData;
	grammarsData.reserve(grammarJson.size());
	for(auto& element : grammarJson)
	{
		GrammarData data;
		data.ID = element.at("id").get<string>();
		data.Name = element.at("name").get<string>();
		data.Type = element.at("type").get<string>();
		
		grammarsData.push_back(std::move(data));
	}

	return grammarsData;
}
GrammarSpecificData WebAPI::GetGrammarSpecificData(const std::string& url, const std::string& grammarID) const
{
	auto grammarJson = PerformGETJSONRequest(url + grammarID);

	GrammarSpecificData grammarData;
	grammarData.ID = grammarJson.at("id").get<string>();
	grammarData.Name = grammarJson.at("name").get<string>();
	grammarData.Type = grammarJson.at("type").get<string>();
	grammarData.Code = Helpers::StringToWString(grammarJson.at("code").get<string>());
	grammarData.Shared = grammarJson.at("shared").get<bool>();
	grammarData.IsOwner = grammarJson.at("isOwner").get<bool>();

	return grammarData;
}

SceneGeometry WebAPI::GetGeometry(const std::string& url, const GrammarSpecificData& data) const
{
	std::string responseHeader;
	std::string responseBody;
	{
		std::string requestBody;
		requestBody += "ID=" + data.ID + "&";
		requestBody += "Name=" + data.Name + "&";
		requestBody += "Type=" + data.Type + "&";
		requestBody += "Code=" + Helpers::WStringToString(data.Code);
		
		CurlList requestHeader;
		if (!PerformPOSTRequest(url, requestHeader, requestBody, responseHeader, responseBody, true))
			ThrowEngineException(L"Failed to perform request.");
	}

	auto dataJson = nlohmann::json::parse(responseBody.c_str());
	Helpers::WriteData(L"Test.txt", responseBody);
	auto objectsArray = dataJson.at("o");

	SceneGeometry scene;
	scene.Objects.reserve(objectsArray.size());
	for (auto& element : objectsArray)
	{
		scene.Objects.push_back(ObjectGeometry::CreateFromJSON(element));
	}

	return scene;
}

CURL* WebAPI::GetCURL()
{
	return m_curl;
}
const CURL* WebAPI::GetCURL() const
{
	return m_curl;
}

bool WebAPI::IsAuthenticated() const
{
	return m_isAuthenticated;
}

void WebAPI::Initialize()
{
	// Initialize windows sockets:
	ThrowIfCURLFailed(curl_global_init(CURL_GLOBAL_DEFAULT));

	m_curl = curl_easy_init();
	if (!m_curl)
		ThrowEngineException(L"Failed to initialize CURL.");

	// Set call back functions:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WebAPI::WriteCallback));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, WebAPI::WriteCallback));

	// Set parameters:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L));
}
void WebAPI::Shutdown()
{
	if (m_curl)
	{
		curl_easy_cleanup(m_curl);
		m_curl = nullptr;
	}

	curl_global_cleanup();
}

bool WebAPI::PerformGETRequest(const std::string& url, std::string& responseHeader, std::string& responseBody, bool setCookie) const
{
	// Set URL:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str()));

	// Clear post flags:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, 0L));

	// Set output for header and body:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));

	// Set cookie if flag is set:
	CurlList header;
	if (setCookie)
		SetCookie(header);
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, header.Get()));

	// Perform request:
	if (curl_easy_perform(m_curl) != CURLE_OK)
		return false;

	return true;
}
bool WebAPI::PerformPOSTRequest(const std::string& url, CurlList& requestHeader, const std::string& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const
{
	// Set URL:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str()));

	// Set post body:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 1L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, requestBody.c_str()));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(requestBody.length())));
	
	// Set output for header and body:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));

	// Set cookie if flag is set:
	if (setCookie)
		SetCookie(requestHeader);
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, requestHeader.Get()));

	// Perform request:
	if (curl_easy_perform(m_curl) != CURLE_OK)
		return false;

	return true;
}
nlohmann::json WebAPI::PerformGETJSONRequest(const std::string& url) const
{
	std::string header;
	std::string body;
	if (!PerformGETRequest(url, header, body, true))
		ThrowEngineException(L"Couldn't perform request");

	// Create json object:
	return nlohmann::json::parse(body.c_str());
}

void WebAPI::AddCookie(const std::string& name, const std::string& value)
{
	auto location = m_cookies.find(name);

	if(m_cookieString.empty())
		m_cookieString += HeaderConstants::Cookie;

	// The cookie doesn't exist. Let's add it:
	if (location == m_cookies.end())
	{
		// Add cookie:
		m_cookies.emplace(name, value);

		// Update cookie string:
		m_cookieString += name + "=" + value + HeaderConstants::Semicolon;
	}
	// The cookie already exist, therefore let's update it:
	else
	{
		// Update cookie value:
		m_cookies[name] = value;

		// Rebuild cookie string:
		m_cookieString.clear();
		m_cookieString += HeaderConstants::Cookie;
		for (auto&& pair : m_cookies)
			m_cookieString += pair.first + "=" + pair.second + HeaderConstants::Semicolon;
	}
}
void WebAPI::SetCookie(CurlList& header) const
{
	header.Append(m_cookieString);
}

int WebAPI::WriteCallback(char* data, size_t size, size_t count, std::string* userData)
{
	if (!userData)
		return 0;

	auto dataSize = size * count;
	userData->append(data, dataSize);

	return static_cast<int>(dataSize);
}

bool WebAPI::ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookieValue)
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

bool WebAPI::ExtractVerificationToken(const std::string& body, std::string& verificationToken)
{
	//	TODO now it works only for direct registrations on the site, it would be good to extend it to external logins (Google, Facebook) if possible. They have a different token and possibly the login sequence is different as well.
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
