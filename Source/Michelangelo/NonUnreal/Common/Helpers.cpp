#include "Michelangelo.h"
#include "Helpers.h"
#include "EngineException.h"

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

std::string Helpers::ArrayToString(const std::array<float, 3>& value)
{
	std::stringstream ss;
	ss << value[0] << "," << value[1] << "," << value[2];
	return ss.str();
}

std::chrono::system_clock::time_point Helpers::ParseTime(const std::string& time, const std::string& format)
{
	std::tm tm = {};
	std::stringstream ss(time);
	ss >> std::get_time(&tm, format.c_str());
	return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

bool Helpers::ParseColor(const nlohmann::json& json, const std::string& name, std::array<float, 4>& output)
{
	auto location = json.find(name);
	if (location == json.end())
		return false;

	const auto& colorArray = *location;
	std::copy(colorArray.begin(), colorArray.end(), output.begin());

	if (colorArray.size() == 3)
		output[3] = 1.0f;

	return true;
}
bool Helpers::ParseFloat(const nlohmann::json& json, const std::string& name, float& output)
{
	auto location = json.find("Shininess");
	if (location == json.end())
		return false;

	output = location->get<float>();

	return true;
}