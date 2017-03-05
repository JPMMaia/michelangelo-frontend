#include "Michelangelo.h"
#include "WebAPI.h"
#include "HeaderConstants.h"
#include "URLConstants.h"
#include "NonUnreal/nlohmann/JSON/json.hpp"
#include "CameraParameters.h"

#include <regex>
#include <limits>

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

WebAPI::LoginError WebAPI::Authenticate(const std::string& email, const std::string& password, bool rememberMe)
{
	// TODO in case of timeout retry
	std::string loginPageBody;
	std::string loginPageHeader;
	if (!PerformGETRequest(URLConstants::LogInAPI, loginPageHeader, loginPageBody, false))
		return LoginError::Unknown;

	// Extract the request verification token:
	std::string requestVerificationToken;
	if (!ExtractLogInVerificationToken(loginPageBody, requestVerificationToken))
		return LoginError::Unknown;

	// Extract the request verification token cookie value:
	std::string requestVerificationTokenCookieValue;
	if (!ExtractCookieValue(loginPageHeader, HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenCookieValue))
		return LoginError::Unknown;
	AddCookie(HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenCookieValue);

	// Perform a post request, sending the login information:
	std::string responseBody;
	std::string responseHeader;
	{
		CurlPost requestBody;
		requestBody.AddPair("__RequestVerificationToken", requestVerificationToken);
		requestBody.AddPair("Email", email);
		requestBody.AddPair("Password", password);
		requestBody.AddPair("RememberMe", rememberMe ? "true" : "false");
		requestBody.Generate(m_curl, false);

		CurlList requestHeader;
		if (!PerformPOSTRequest(URLConstants::LogInAPI, requestHeader, requestBody, responseHeader, responseBody, true))
			return LoginError::Unknown;
	}

	// Extract the application cookie:
	std::string applicationCookieValue;
	if (!ExtractCookieValue(responseHeader, HeaderConstants::ApplicationCookieName, applicationCookieValue))
		return LoginError::WrongCredentials;
	AddCookie(HeaderConstants::ApplicationCookieName, applicationCookieValue);

	m_isAuthenticated = true;

	return LoginError::None;
}

void WebAPI::LogOut()
{
	if (!m_isAuthenticated)
		return;

	// Get body of the main page:
	std::string mainPageHeader;
	std::string mainPageBody;
	if (!PerformGETRequest(URLConstants::MainPage, mainPageHeader, mainPageBody, true))
		return;

	// Extract the request verification token:
	std::string requestVerificationTokenValue;
	if (!ExtractLogOutVerificationToken(mainPageBody, requestVerificationTokenValue))
		return;

	// Generate request body:
	CurlPost requestBody;
	requestBody.AddPair(HeaderConstants::RequestVerificationTokenCookieName, requestVerificationTokenValue);
	requestBody.Generate(m_curl, true);

	// Perform logout:
	std::string responseBody;
	std::string responseHeader;
	CurlList requestHeader;
	if (!PerformPOSTRequest(URLConstants::LogOutAPI, requestHeader, requestBody, responseHeader, responseBody, true))
		return; // TODO internal error?

	m_isAuthenticated = false;
}

GrammarSpecificData MichelangeloAPI::WebAPI::CreateNewGrammar() const
{
	CurlList requestHeader;
	CurlPost requestBody;
	std::string responseHeader, responseBody;
	if (!PerformPUTRequest(URLConstants::OwnGrammarAPI, requestHeader, requestBody, responseHeader, responseBody, true))
		ThrowEngineException(L"Couldn't perform PUT request");

	return GrammarSpecificData::FromJson(nlohmann::json::parse(responseBody.c_str()));
}
void MichelangeloAPI::WebAPI::DeleteGrammar(const std::string & id) const
{
	auto url = URLConstants::OwnGrammarAPI + id;
	std::string responseHeader, responseBody;
	if (!PerformDELETERequest(url, responseHeader, responseBody, true))
		ThrowEngineException(L"Couldn't perform DELETE request");
}
void MichelangeloAPI::WebAPI::ShareGrammar(const std::string& id, bool share) const
{
	auto url = URLConstants::OwnGrammarAPI + "share/" + (share ? "true" : "false") + "/" + id;
	CurlList requestHeader;
	CurlPost requestBody;
	{
		requestBody.AddPair("share", (share ? "true" : "false"));
		requestBody.Generate(m_curl, true);
	}
	std::string responseHeader, responseBody;
	if (!PerformPUTRequest(url, requestHeader, requestBody, responseHeader, responseBody, true))
		ThrowEngineException(L"Couldn't perform PUT request");
}

std::vector<GrammarSpecificData> WebAPI::GetGrammars(const std::string& url) const
{
	auto grammarJson = PerformGETJSONRequest(url);

	std::vector<GrammarSpecificData> grammarsData;
	grammarsData.reserve(grammarJson.size());
	for (const auto& element : grammarJson)
		grammarsData.push_back(GrammarSpecificData::FromJson(element));

	return grammarsData;
}

GrammarSpecificData WebAPI::GetGrammarSpecificData(const std::string& url, const std::string& grammarID) const
{
	return GrammarSpecificData::FromJson(PerformGETJSONRequest(url + grammarID));
}

bool WebAPI::GetGeometry(const std::string& url, const GrammarSpecificData& data, const CameraParameters& cameraParameters, SceneGeometry& sceneGeometry, std::string& errorMessage) const
{
	// Perform request:
	std::string responseBody;
	{
		CurlPost requestBody;
		requestBody.AddPair("ID", data.ID);
		requestBody.AddPair("Name", data.Name);
		requestBody.AddPair("Type", data.Type);
		requestBody.AddPair("Code", Helpers::WStringToString(data.Code));
		requestBody.AddPair("OnlyNID", std::to_string(std::numeric_limits<unsigned int>::max()));
		requestBody.AddPair("CamPos", Helpers::ArrayToString(cameraParameters.GetCameraPosition()));
		requestBody.AddPair("CamUp", Helpers::ArrayToString(cameraParameters.GetCameraUpDirection()));
		requestBody.AddPair("CamLook", Helpers::ArrayToString(cameraParameters.GetCameraUpDirection()));
		requestBody.AddPair("CamFOV", std::to_string(cameraParameters.GetCameraFov()));
		requestBody.AddPair("Width", std::to_string(cameraParameters.GetWidth()));
		requestBody.AddPair("Height", std::to_string(cameraParameters.GetHeight()));

		requestBody.Generate(m_curl, true);

		CurlList requestHeader;
		std::string responseHeader;
		if (!PerformPOSTRequest(url, requestHeader, requestBody, responseHeader, responseBody, true))
			ThrowEngineException(L"Failed to perform request.");
	}

	// Parse response json:
	auto dataJson = nlohmann::json::parse(responseBody.c_str());

	// Find if any unexpected error occurred:
	{
		auto location = dataJson.find("message");
		if (location != dataJson.end())
		{
			errorMessage = location->get<std::string>();
			return false;
		}
	}

	// Find if gramatical errors occurred (including warnings):
	errorMessage = dataJson.at("e").get<std::string>();

	// Create geometry from json:
	sceneGeometry = SceneGeometry::CreateFromJson(dataJson);

	// If there is an error message and no geometry was created, then it was a critical error (not only warnings):
	if (!errorMessage.empty() && sceneGeometry.GetObjects().empty())
		return false;

	return true;
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

bool WebAPI::PerformPOSTRequest(const std::string& url, CurlList& requestHeader, const CurlPost& requestBody, std::string& responseHeader, std::string& responseBody, bool setCookie) const
{
	// Set URL:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str()));

	// Set output for header and body:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));

	// Set cookie if flag is set:
	if (setCookie)
		SetCookie(requestHeader);
	requestHeader.Append("Content-Type: application/x-www-form-urlencoded");
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, requestHeader.Get()));

	// Set post body:
	{
		const auto& postBody = requestBody.GetData();
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 1L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, postBody.size()));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postBody.c_str()));
	}

	// Perform request:
	auto error = curl_easy_perform(m_curl);

	// Clear post flags:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, 0L));

	if (error != CURLE_OK)
		return false;

	return true;
}
bool MichelangeloAPI::WebAPI::PerformPUTRequest(const std::string & url, CurlList& requestHeader, const CurlPost& requestBody, std::string & responseHeader, std::string & responseBody, bool setCookie) const
{
	// Set URL:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str()));

	// Set output for header and body:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));

	// Set cookie if flag is set:
	if (setCookie)
		SetCookie(requestHeader);
	requestHeader.Append("Content-Type: application/x-www-form-urlencoded");
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, requestHeader.Get()));

	// Set PUT method:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "PUT"));
	
	// Set data:
	{
		auto requestBodyContent = requestBody.GetData();
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, requestBodyContent.size()));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, requestBodyContent.c_str()));
	}

	// Perform request:
	auto error = curl_easy_perform(m_curl);

	// Restore state to default:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPGET, 1L));

	if (error != CURLE_OK)
		return false;

	return true;
}
bool MichelangeloAPI::WebAPI::PerformDELETERequest(const std::string& url, std::string& responseHeader, std::string& responseBody, bool setCookie) const
{
	// Set URL:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str()));

	// Set output for header and body:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));

	// Set cookie if flag is set:
	CurlList requestHeader;
	if (setCookie)
		SetCookie(requestHeader);
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, requestHeader.Get()));

	// Set DELETE method:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "DELETE"));

	// Perform request:
	auto error = curl_easy_perform(m_curl);

	// Restore method to default:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPGET, 1L));

	if (error != CURLE_OK)
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

	if (m_cookieString.empty())
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

bool WebAPI::ExtractLogInVerificationToken(const std::string& body, std::string& verificationToken)
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

bool WebAPI::ExtractLogOutVerificationToken(const std::string& body, std::string& verificationToken)
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
