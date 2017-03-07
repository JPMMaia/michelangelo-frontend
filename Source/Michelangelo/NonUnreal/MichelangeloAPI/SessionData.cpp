#include "Michelangelo.h"
#include "SessionData.h"

#include <sstream>

using namespace MichelangeloAPI;

void SessionData::AddCookie(const std::string& name, const std::string& value)
{
	// Lock cookies mutex:
	std::lock_guard<std::mutex> lock(m_cookiesMutex);

	// Search for the cookie name:
	auto location = m_cookies.find(name);

	// The cookie doesn't exist. Let's add it:
	if (location == m_cookies.end())
	{
		m_cookies.emplace(name, value);
	}

	// The cookie already exist, therefore let's update it:
	else
	{
		m_cookies[name] = value;
	}

	// Rebuild cookie string:
	BuildCookieString();
}
void SessionData::ClearCookies()
{
	// Lock cookies mutex:
	std::lock_guard<std::mutex> lock(m_cookiesMutex);

	m_cookieString.clear();
	m_cookies.clear();
}
const std::string& SessionData::GetCookies() const
{
	return m_cookieString;
}

void SessionData::IsAuthenticated(bool value)
{
	m_isAuthenticated = value;
}
bool SessionData::IsAuthenticated() const
{
	return m_isAuthenticated;
}

CurlList SessionData::GenerateHeaderWithCookies() const
{
	CurlList header;
	header.Append(m_cookieString);
	return header;
}

void SessionData::BuildCookieString()
{
	std::stringstream ss;
	ss << "Cookie:";

	for (auto&& pair : m_cookies)
		ss << pair.first << "=" << pair.second << "; ";

	m_cookieString = ss.str();
}
