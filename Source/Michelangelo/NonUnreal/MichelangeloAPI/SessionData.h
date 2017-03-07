#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include "CurlList.h"

namespace MichelangeloAPI
{
	class SessionData
	{
	public:
		void AddCookie(const std::string& name, const std::string& value);
		void ClearCookies();
		const std::string& GetCookies() const;

		void IsAuthenticated(bool value);
		bool IsAuthenticated() const;

		CurlList GenerateHeaderWithCookies() const;

	private:
		void BuildCookieString();

	private:
		std::mutex m_cookiesMutex;
		std::unordered_map<std::string, std::string> m_cookies;
		std::string m_cookieString;

		std::atomic<bool> m_isAuthenticated = false;
	};
}
