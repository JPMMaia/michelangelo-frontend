#include "Michelangelo.h"
#include "CurlPost.h"
#include "NonUnreal/Common/EngineException.h"

using namespace Common;
using namespace MichelangeloAPI;

CurlPost::~CurlPost()
{
	if (m_first)
	{
		curl_formfree(m_first);
	}
}

void CurlPost::AddPair(const std::string& name, const std::string& value)
{
	m_pairs[name] = value;
	m_dirty = true;
}
void CurlPost::GenerateHttpPost()
{
	if (!m_dirty)
		return;

	for (const auto& pair : m_pairs)
	{
		auto result =
			curl_formadd(
				&m_first, &m_last,
				CURLFORM_PTRNAME, pair.first.c_str(),
				CURLFORM_PTRCONTENTS, pair.second.c_str(),
				CURLFORM_END
			);

		if (result != 0)
			ThrowEngineException(L"Failed");
	}

	m_dirty = false;
}
const curl_httppost* CurlPost::Get() const
{
	return m_first;
}
