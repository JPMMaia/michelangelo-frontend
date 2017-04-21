#include "Michelangelo.h"
#include "CurlHandle.h"
#include "CurlException.h"
#include "CurlList.h"
#include "CurlBody.h"

using namespace MichelangeloAPI;

CurlHandle::CurlHandle()
{
	m_handle = curl_easy_init();
	assert(m_handle);

	// Set call back functions:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, CurlHandle::WriteCallback));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION, CurlHandle::WriteCallback));

	// Set parameters:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_SSL_VERIFYPEER, 0L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 1L));
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
	InitializeRequest(url, requestHeader, responseHeader, responseBody);

	// Perform request:
	if (curl_easy_perform(m_handle) != CURLE_OK)
		ThrowEngineException(L"Couldn't perform GET request.");
}
void CurlHandle::PerformPOSTRequest(const std::string& url, const CurlList& requestHeader, const CurlBody& requestBody, std::string& responseHeader, std::string& responseBody) const
{
	InitializeRequest(url, requestHeader, responseHeader, responseBody);

	// Set request body content:
	{
		const auto& requestBodyContent = requestBody.GetData();
		CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POST, 1L));
		CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, requestBodyContent.size()));
		CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, requestBodyContent.c_str()));
	}

	// Perform request:
	auto errorCode = curl_easy_perform(m_handle);

	// Clear post flags:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POST, 0L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, 0L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_HTTPGET, 1L));

	if (errorCode != CURLE_OK)
		ThrowEngineException(L"Couldn't perform POST request.");
}
void CurlHandle::PerformPUTRequest(const std::string& url, const CurlList& requestHeader, const CurlBody& requestBody, std::string& responseHeader, std::string& responseBody) const
{
	InitializeRequest(url, requestHeader, responseHeader, responseBody);

	// Set PUT method:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, "PUT"));

	// Set request body content:
	{
		const auto& requestBodyContent = requestBody.GetData();
		CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, requestBodyContent.size()));
		CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, requestBodyContent.c_str()));
	}

	// Perform request:
	auto errorCode = curl_easy_perform(m_handle);

	// Restore state to default:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, 0L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, 0L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_HTTPGET, 1L));

	if (errorCode != CURLE_OK)
		ThrowEngineException(L"Couldn't perform PUT request.");
}
void CurlHandle::PerformDELETERequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const
{
	InitializeRequest(url, requestHeader, responseHeader, responseBody);

	// Set DELETE method:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, "DELETE"));

	// Perform request:
	auto errorCode = curl_easy_perform(m_handle);

	// Restore state to default:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, 0L));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_HTTPGET, 1L));

	if (errorCode != CURLE_OK)
		ThrowEngineException(L"Couldn't perform DELETE request.");
}

CURL* CurlHandle::Get() const
{
	return m_handle;
}

int CurlHandle::WriteCallback(char* data, size_t size, size_t count, std::string* userData)
{
	if (!userData)
		return 0;

	auto dataSize = size * count;
	userData->append(data, dataSize);

	return static_cast<int>(dataSize);
}

void CurlHandle::InitializeRequest(const std::string& url, const CurlList& requestHeader, std::string& responseHeader, std::string& responseBody) const
{
	// Set URL:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str()));

	// Set output for header and body:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_HEADERDATA, &responseHeader));
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, &responseBody));

	// Set request header:
	CurlAssert(curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, requestHeader.Get()));
}
