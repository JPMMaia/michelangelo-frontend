#include "Michelangelo.h"
#include "CurlList.h"
#include "IncludeCurl.h"

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
