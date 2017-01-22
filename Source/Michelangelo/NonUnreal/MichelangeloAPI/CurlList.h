#pragma once

#include <string>

struct curl_slist;

namespace MichelangeloAPI
{
	class CurlList
	{
	public:
		CurlList() = default;
		~CurlList();

		void Append(const std::string& value);
		void Clear();

		curl_slist* Get();
		const curl_slist* Get() const;

	private:
		curl_slist* m_list = nullptr;
	};
}