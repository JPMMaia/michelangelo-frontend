#pragma once

#include "IncludeCurl.h"

#include <string>
#include <unordered_map>

namespace MichelangeloAPI
{
	class CurlPost
	{
	public:
		~CurlPost();

		void AddPair(const std::string& name, const std::string& value);
		void GenerateHttpPost();
		const curl_httppost* Get() const;

	private:
		std::unordered_map<std::string, std::string> m_pairs;
		curl_httppost* m_first = nullptr;
		curl_httppost* m_last = nullptr;
		bool m_dirty = true;
	};
}
