#include "Michelangelo.h"
#include "CurlList.h"

#ifndef NATIVE_CPP
// ReSharper disable once CppUnusedIncludeDirective
#include <AllowWindowsPlatformTypes.h>
#endif

#include <curl/curl.h>
#undef PF_MAX

#ifndef NATIVE_CPP
// ReSharper disable once CppUnusedIncludeDirective
#include <HideWindowsPlatformTypes.h>
#endif

using namespace MichelangeloAPI;

CurlList::~CurlList()
{
	Clear();
}

void CurlList::Append(const std::string& value)
{
	m_list = curl_slist_append(m_list, value.c_str());
}

void CurlList::Clear()
{
	if (m_list)
	{
		curl_slist_free_all(m_list);
		m_list = nullptr;
	}
}

curl_slist* CurlList::Get()
{
	return m_list;
}
const curl_slist* CurlList::Get() const
{
	return m_list;
}
