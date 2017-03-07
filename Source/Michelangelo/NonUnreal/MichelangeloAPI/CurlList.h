#pragma once

#include <memory>
#include <string>

#include "IncludeCurl.h"

struct curl_slist;

namespace MichelangeloAPI
{
	class CurlList
	{
	public:
		CurlList();

		void Append(const std::string& value);
		void Clear();

		curl_slist* Get();
		const curl_slist* Get() const;

	private:
		using CleanupFunctionType = void(*)(curl_slist*);
		using CurlListHandle = std::unique_ptr<curl_slist, CleanupFunctionType>;
		CurlListHandle m_list = nullptr;
	};
}