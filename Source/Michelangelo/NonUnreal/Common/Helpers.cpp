#include "Michelangelo.h"
#include "Helpers.h"
#include "EngineException.h"

#ifndef NATIVE_CPP
// ReSharper disable once CppUnusedIncludeDirective
#include <AllowWindowsPlatformTypes.h>
#endif

#include <comdef.h>

#ifndef NATIVE_CPP
// ReSharper disable once CppUnusedIncludeDirective
#include <HideWindowsPlatformTypes.h>
#endif

using namespace Common;

bool Helpers::FileExists(const std::wstring& filename)
{
	std::fstream fileStream(filename);

	return fileStream.good();
}

std::wstring Helpers::GetFilename(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto begin = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (begin == filename.end())
		begin = filename.begin();
	else
		++begin;

	std::wstring dot(L".");
	auto end = std::find_end(begin, filename.end(), dot.begin(), dot.end());

	return std::wstring(begin, end);
}
std::wstring Helpers::GetFileExtension(const std::wstring& filename)
{
	std::wstring dot(L".");
	auto end = std::find_end(filename.begin(), filename.end(), dot.begin(), dot.end());
	if (end == filename.end())
		ThrowEngineException(L"Filename has no extension.");

	return std::wstring(end + 1, filename.end());
}
std::wstring Helpers::GetFilePath(const std::wstring& filename)
{
	std::wstring searchString(L"/");

	auto location = std::find_end(filename.begin(), filename.end(), searchString.begin(), searchString.end());
	if (location == filename.end())
		return L"";

	return std::wstring(filename.begin(), location + 1);
}

std::wstring Helpers::StringToWString(const std::string& str)
{
	using convertType = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convertType, wchar_t> converter;

	return converter.from_bytes(str);
}

std::string Helpers::WStringToString(const std::wstring& wstr)
{
	using convertType = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convertType, wchar_t> converter;

	return converter.to_bytes(wstr);
}

std::string Helpers::EscapeString(CURL* curl, const std::string& value)
{	
	// URL Escape:
	auto escapedCString = curl_easy_escape(curl, value.c_str(), value.size());

	// Convert C string to std::string:
	std::string escapedString(escapedCString);

	// Free C string:
	curl_free(escapedCString);

	return escapedString;
}
