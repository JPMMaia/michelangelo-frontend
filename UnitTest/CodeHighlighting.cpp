#include "stdafx.h"
#include "CppUnitTest.h"
#include "NonUnreal/Common/CSharpRegexHelper.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(CSharpRegexUnitTestSet)
	{
	public:
		
		TEST_METHOD(CSharpRegexUnitTest1)
		{
			CSharpRegexHelper regexHelper(L"Highlighting.json");

			const auto& keywordsRegex = regexHelper.GetKeywordsRegex();
			
			std::string expression = R"code(
			public void Add()
			{
				float x = 2.0f; 
			}
			)code";
			std::array<std::string, 3> expectedMatches = { "public", "void", "float" };
			
			size_t index = 0;
			std::smatch matches;

			while(index < expectedMatches.size())
			{
				std::regex_search(expression, matches, keywordsRegex);
				
				Assert::IsTrue(expectedMatches[index++] == *matches.begin());
				expression = matches.suffix().str();
			}
		}

	};
}