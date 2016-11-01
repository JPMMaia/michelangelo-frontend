#include "MichelangeloPluginPrivatePCH.h"
#include "WebAPI.h"
#include "HeaderConstants.h"
#include "URLConstants.h"
#include "nlohmann/JSON/json.hpp"

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
	// Set call back functions:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WebAPI::WriteCallback));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, WebAPI::WriteCallback));

	// Set parameters:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, URLConstants::LoginPage.c_str()));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L));

	// TODO in case of timeout retry
	std::string loginPageBody;
	std::string loginPageHeader;
	{
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &loginPageBody));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &loginPageHeader));
		ThrowIfCURLFailed(curl_easy_perform(m_curl));
	}

	// Extract the request verification token:
	std::string verificationToken; 
	if (!ExtractVerificationToken(loginPageBody, verificationToken))
		ThrowEngineException(L"Failed to extract verification token.");

	std::string verificationCookieValue;
	if (!ExtractCookieValue(loginPageHeader, HeaderConstants::RequestCookie, verificationCookieValue))
		ThrowEngineException(L"Failed to extract verification cookie.");

	std::string responseBody;
	std::string responseHeader;
	{
		// Put the cookie token into the header:
		auto headerCookie = BuildCookie({ verificationCookieValue });
		SListHandle headerChunk(curl_slist_append(nullptr, headerCookie.c_str()), &curl_slist_free_all);
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headerChunk.get()));

		// The credentials go here, plus the body request token:
		//TODO replace the LOGIN and PWD by the username and password of the user!!! Otherwise it wil not work!
		auto postBody = "__RequestVerificationToken=" + verificationToken + "&Email=" + username + "&Password=" + password + "&RememberMe=" + (rememberMe ? "true" : "false");
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 1L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postBody.c_str()));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(postBody.length())));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));

		// Now we will authorize the user and receive back an application token:
		if(curl_easy_perform(m_curl) != CURLE_OK)
			return false;
	}

	std::string applicationCookieValue; 
	if (!ExtractCookieValue(responseHeader, HeaderConstants::ApplicationCookie, applicationCookieValue))
		ThrowEngineException(L"Failed to extract application cookie");

	// Set application cookie:
	auto cookie = BuildCookie({ verificationCookieValue, applicationCookieValue });
	m_cookie = curl_slist_append(m_cookie, cookie.c_str());

	return true;
}

std::vector<TutorialData> WebAPI::GetTutorials() const
{
	std::string header;
	std::string tutorialsJsonString;
	{
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, URLConstants::TutorialsAPI.c_str()));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 0L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, 0L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WebAPI::WriteCallback));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, WebAPI::WriteCallback));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &tutorialsJsonString));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &header));
		SetCookie();
		ThrowIfCURLFailed(curl_easy_perform(m_curl));
	}

	// Create json object:
	using json = nlohmann::json;
	auto tutorialsJson = json::parse(tutorialsJsonString.c_str());

	std::vector<TutorialData> tutorialsData;
	tutorialsData.reserve(tutorialsJson.size());
	for(auto& element : tutorialsJson)
	{
		TutorialData data;
		data.ID = element.at("id").get<string>();
		data.Name = element.at("name").get<string>();
		tutorialsData.push_back(std::move(data));
	}

	return tutorialsData;
}

CURL* WebAPI::GetCURL()
{
	return m_curl;
}
const CURL* WebAPI::GetCURL() const
{
	return m_curl;
}

curl_slist* WebAPI::GetCookie()
{
	return m_cookie;
}
const curl_slist* WebAPI::GetCookie() const
{
	return m_cookie;
}

void WebAPI::Initialize()
{
	// Initialize windows sockets:
	ThrowIfCURLFailed(curl_global_init(CURL_GLOBAL_DEFAULT));

	m_curl = curl_easy_init();
	if (!m_curl)
		ThrowEngineException(L"Failed to initialize CURL.");
}
void WebAPI::Shutdown()
{
	if (m_cookie)
	{
		curl_slist_free_all(m_cookie);
		m_cookie = nullptr;
	}

	if (m_curl)
	{
		curl_easy_cleanup(m_curl);
		m_curl = nullptr;
	}

	curl_global_cleanup();
}
void WebAPI::SetCookie() const
{
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_cookie));
}

int WebAPI::WriteCallback(char* data, size_t size, size_t count, std::string* userData)
{
	if (!userData)
		return 0;

	auto dataSize = size * count;
	userData->append(data, dataSize);

	return static_cast<int>(dataSize);
}

bool WebAPI::ExtractCookieValue(const std::string& header, const std::string& cookieName, std::string& cookie)
{
	// Check if there's a set-cookie in the response:
	if (header.find(HeaderConstants::SetCookie) == std::string::npos)
		return false;

	// Extract the cookie:
	auto start = header.find(cookieName);
	if (start == std::string::npos)
		return false;

	auto end = header.find(HeaderConstants::Semicolon, start);
	cookie = header.substr(start, end - start);

	return true;
}

std::string WebAPI::BuildCookie(const std::initializer_list<std::string>& cookieValues)
{
	std::string cookie;

	cookie += HeaderConstants::Cookie;

	for(auto& cookieValue : cookieValues)
		cookie += cookieValue + HeaderConstants::Semicolon;

	return cookie;
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
