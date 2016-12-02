#include "Michelangelo.h"
#include "Helpers.h"
#include "EngineException.h"

#include "AllowWindowsPlatformTypes.h"
#include <comdef.h>
#include "HideWindowsPlatformTypes.h"

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
	if(end == filename.end())
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
std::string Helpers::FStringToString(const FString& fstr)
{
	return std::string(TCHAR_TO_UTF8(*fstr));
}
FString Helpers::WStringToFString(const std::wstring& wstr)
{
	return StringToFString(WStringToString(wstr));
}
std::wstring Helpers::FStringToWString(const FString& fstr)
{
	return StringToWString(FStringToString(fstr));
}
FString Helpers::StringToFString(const std::string& str)
{
	return FString(str.c_str());
}
FMatrix Helpers::ArrayToMatrix(const std::array<float, 16>& transformArray)
{
	return FMatrix(
		FPlane(transformArray[0], transformArray[1], transformArray[2], transformArray[3]),
		FPlane(transformArray[4], transformArray[5], transformArray[6], transformArray[7]),
		FPlane(transformArray[8], transformArray[9], transformArray[10], transformArray[11]),
		FPlane(transformArray[12], transformArray[13], transformArray[14], transformArray[15])
	);
	/*return FMatrix(
		FPlane(transformArray[0], transformArray[4], transformArray[8], transformArray[12]),
		FPlane(transformArray[1], transformArray[5], transformArray[9], transformArray[13]),
		FPlane(transformArray[2], transformArray[6], transformArray[10], transformArray[14]),
		FPlane(transformArray[3], transformArray[7], transformArray[11], transformArray[15])
	);*/
}

void Common::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		_com_error error(hr);

		// Set a breakpoint on this line to catch Win32 API errors.
		ThrowEngineException(error.ErrorMessage());
	}
}
