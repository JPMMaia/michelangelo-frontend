#pragma once

#include <array>
#include <string>

namespace Common
{
	namespace Helpers
	{
		std::string FStringToString(const FString& fstr);
		FString WStringToFString(const std::wstring& wstr);
		std::wstring FStringToWString(const FString& fstr);
		FString StringToFString(const std::string& str);
		FMatrix ArrayToMatrix(const std::array<float, 16>& transformArray);
	}
}