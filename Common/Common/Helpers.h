#pragma once

#include <codecvt>
#include <future>
#include <fstream>
#include <windows.h>
#include "EngineException.h"

namespace Common
{
	namespace Helpers
	{
		bool FileExists(const std::wstring& filename);
		std::wstring GetFilename(const std::wstring& filename);
		std::wstring GetFileExtension(const std::wstring& filename);
		std::wstring GetFilePath(const std::wstring& filename);

		std::wstring StringToWString(const std::string& str);
		std::string WStringToString(const std::wstring& wstr);

		void ReadData(const std::wstring& filename, std::vector<char>& buffer);
		void WriteData(const std::wstring& filename, const std::vector<char>& buffer);

		template<typename FunctionType, typename... ArgumentsType>
		std::future<typename std::result_of<FunctionType(ArgumentsType...)>::type> RunAsync(FunctionType&& function, ArgumentsType&&... arguments)
		{
			return std::async(std::launch::async, std::forward<FunctionType>(function), std::forward<ArgumentsType>(arguments)...);
		}

		inline std::wstring AnsiToWString(const std::string& str)
		{
			WCHAR buffer[512];
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
			return std::wstring(buffer);
		}
	}

	void ThrowIfFailed(HRESULT hr);
}
