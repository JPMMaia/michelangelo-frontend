#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TextHighlight;

namespace UnitTest
{
	TEST_CLASS(CSharpHighlightSettingsTestSet)
	{
	public:

		TEST_METHOD(CSharpHighlightSettingsTest1)
		{
			std::string input("public void Add(float value1, float value2)");
			std::array<std::string, 4> expectedOutput = { "public", "void", "float", "float" };

			const auto& regex = CSharpHighlightSettings::Get()->GetKeywordsRegex();
			
			for(size_t i = 0; i < expectedOutput.size(); ++i)
			{
				std::smatch match;
				Assert::IsTrue(std::regex_search(input, match, regex));
				Assert::AreEqual(expectedOutput[i], static_cast<std::string>(match[0]));
				input = match.suffix().str();
			}
		}
	};
}