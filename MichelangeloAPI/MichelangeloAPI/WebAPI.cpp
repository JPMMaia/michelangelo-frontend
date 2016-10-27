#include "WebAPI.h"
#include "HeaderConstants.h"

#include <regex>

using namespace Common;
using namespace MichelangeloAPI;

int WriteCallback(char* data, size_t size, size_t nmemb, std::string* writerData)
{
	if (writerData == nullptr)
		return 0;

	writerData->append(data, size * nmemb);
	return static_cast<int>(size * nmemb);
}

WebAPI::WebAPI()
{
	Initialize();
}
WebAPI::~WebAPI()
{
	Shutdown();
}

void WebAPI::Authenticate()
{
	// Set call back functions:
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback));
	ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, WriteCallback));

	// TODO in case of timeout retry
	std::string loginPageBody;
	std::string loginPageHeader;
	{
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_URL, "https://michelangelo.graphics/Account/Login"));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &loginPageBody));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &loginPageHeader));

		ThrowIfCURLFailed(curl_easy_perform(m_curl));
	}

	// Extract the request verification token:
	std::string requestVerificationToken;
	{
		//	TODO now it works only for direct registrations on the site, it would be good to extend it to external logins (Google, Facebook) if possible. They have a different token and possibly the login sequence is different as well.
		std::regex tokenRegex("form action=\"\\/Account\\/Login\" .*><input name=\"__RequestVerificationToken\" type=\"hidden\" value=\".*\"");
		std::smatch match;
		std::string tag;
		std::regex_search(loginPageBody, match, tokenRegex);
		for (auto x : match) tag = x;

		// Now there should be quite a large substring in the tag including the form and its first input field (which bears the token)
		std::regex tokenValue_regex("value=\".*\""); // Match the value of the token
		std::regex_search(tag, match, tokenValue_regex);
		for (auto x : match) tag = x;

		requestVerificationToken = tag.substr(7, tag.length() - 8);
	}

	std::string verificationCookieValue;
	if (!ExtractCookieValue(loginPageHeader, HeaderConstants::RequestCookie, verificationCookieValue))
		ThrowEngineException(L"Failed to extract verification cookie");

	std::string responseHeader;
	{
		// Put the cookie token into the header:
		auto headerCookie = BuildCookie({ verificationCookieValue });
		SListHandle headerChunk(curl_slist_append(nullptr, headerCookie.c_str()), &curl_slist_free_all);
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headerChunk.get()));

		// The credentials go here, plus the body request token:
		//TODO replace the LOGIN and PWD by the username and password of the user!!! Otherwise it wil not work!
		auto postBody = "__RequestVerificationToken=" + requestVerificationToken + "&Email=jpmmaia@gmail.com&Password=yslxqCIVAIqYFuAqYUImyNo5375NYhVGyVwdNerkrjnV8HMEPnwiyQBISnSAThj5&RememberMe=false";
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POST, 1L));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postBody.c_str()));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(postBody.length())));
		std::string responseBody;
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody));
		ThrowIfCURLFailed(curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader));

		// Now we will authorize the user and receive back an application token:
		ThrowIfCURLFailed(curl_easy_perform(m_curl));
	}

	std::string applicationCookieValue; 
	if (!ExtractCookieValue(responseHeader, HeaderConstants::ApplicationCookie, applicationCookieValue))
		ThrowEngineException(L"Failed to extract application cookie");

	// Set application cookie:
	auto cookie = BuildCookie({ verificationCookieValue, applicationCookieValue });
	m_cookie = curl_slist_append(m_cookie, cookie.c_str());
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
