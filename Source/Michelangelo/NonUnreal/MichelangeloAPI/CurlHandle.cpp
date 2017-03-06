#include "Michelangelo.h"
#include "CurlHandle.h"
#include "CurlException.h"

using namespace MichelangeloAPI;

CurlHandle::CurlHandle()
{
	m_handle = curl_easy_init();
	if (!m_handle)
		ThrowEngineException(L"Failed to initialize CURL.");

	// Set call back functions:
	ThrowIfCURLFailed(curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, NativeWebAPI::WriteCallback));
	ThrowIfCURLFailed(curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION, NativeWebAPI::WriteCallback));

	// Set parameters:
	ThrowIfCURLFailed(curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYPEER, 0L));
	ThrowIfCURLFailed(curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L));
	ThrowIfCURLFailed(curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 1L));
}
CurlHandle::~CurlHandle()
{
	if (m_handle)
	{
		curl_easy_cleanup(m_handle);
		m_handle = nullptr;
	}
}

void CurlHandle::PerformGETRequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const
{

}

int CurlHandle::WriteCallback(char* data, size_t size, size_t count, std::string* userData)
{
	if (!userData)
		return 0;

	auto dataSize = size * count;
	userData->append(data, dataSize);

	return static_cast<int>(dataSize);
}
