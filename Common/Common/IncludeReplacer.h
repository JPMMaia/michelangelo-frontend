#pragma once

#include <vector>

namespace Common
{
	class IncludeReplacer
	{
	public:
		static void ReplaceIncludes(const std::wstring& inputFilename, const std::wstring& outputFilename);

	private:
		static bool FindAndEraseIncludeTag(std::vector<char>& content, std::wstring& includedFile, std::vector<char>::iterator& location);
	};
}
