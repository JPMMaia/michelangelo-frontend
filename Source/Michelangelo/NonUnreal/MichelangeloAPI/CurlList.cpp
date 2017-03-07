#include "Michelangelo.h"
#include "CurlList.h"
#include "IncludeCurl.h"

using namespace MichelangeloAPI;

CurlList::CurlList() :
	m_list(nullptr, &curl_slist_free_all)
{
}

void CurlList::Append(const std::string& value)
{
	m_list = CurlListHandle(curl_slist_append(m_list.get(), value.c_str()), &curl_slist_free_all);
}
void CurlList::Clear()
{
	if (m_list)
	{
		m_list.reset();
	}
}

curl_slist* CurlList::Get()
{
	return m_list.get();
}
const curl_slist* CurlList::Get() const
{
	return m_list.get();
}
