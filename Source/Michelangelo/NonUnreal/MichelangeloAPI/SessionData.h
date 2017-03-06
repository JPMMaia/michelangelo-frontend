#pragma once

#include <string>
#include <unordered_map>

namespace MichelangeloAPI
{
	class SessionData
	{
	public:


		bool IsAuthenticated() const;

	private:
		std::unordered_map<std::string, std::string> m_cookies;
		std::string m_cookieString;
		bool m_isAuthenticated = false;
	};
}