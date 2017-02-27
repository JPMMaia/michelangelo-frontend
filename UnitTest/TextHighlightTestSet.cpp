#include "stdafx.h"
#include "CppUnitTest.h"
#include "NonUnreal/TextHighlight/CSharpCodeHighlighter.hpp"
#include "NonUnreal/TextHighlight/MixTextPiece.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TextHighlight;

namespace UnitTest
{
	TEST_CLASS(TextHighlightTestSet)
	{
	public:

		TEST_METHOD(TextHighlightTest1)
		{
			MultiLineCommentPiece input("/* Multi\nLine\nComment\n*/\n");
			std::string expectedOutput("<span color=\"#48A433\">/* Multi</>\n<span color=\"#48A433\">Line</>\n<span color=\"#48A433\">Comment</>\n<span color=\"#48A433\">*/</>\n");
			Assert::AreEqual(expectedOutput, input.ToString());
		}

		TEST_METHOD(TextHighlightTest2)
		{
			std::string input("public void Add(float value1, float value2)");
			std::string expectedOutput("<span color=\"#4A9A9B\">public</> <span color=\"#4A9A9B\">void</> Add(<span color=\"#4A9A9B\">float</> value1, <span color=\"#4A9A9B\">float</> value2)");

			MixTextPiece parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTest3)
		{
			std::string input("var value = 0.0f; // Declaring var");
			std::string expectedOutput("<span color=\"#4A9A9B\">var</> value = 0.0f; <span color=\"#48A433\">// Declaring var</>");

			MixTextPiece parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTest4)
		{
			std::string input("var value1 = 0.0f; /* Declaring\nvar\n*/\nvar value2 = 0.0f;\n");
			std::string expectedOutput("<span color=\"#4A9A9B\">var</> value1 = 0.0f; <span color=\"#48A433\">/* Declaring</>\n<span color=\"#48A433\">var</>\n<span color=\"#48A433\">*/</>\n<span color=\"#4A9A9B\">var</> value2 = 0.0f;\n");

			MixTextPiece parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTest5)
		{
			std::string input(R"text(
				/*
				 * Add two numbers.
				 */
				public void Add(float value1, float value2)
				{
					// Adding values:
					return value1 + value2;	
				}
			)text");
			std::string expectedOutput(R"text(
				<span color="#48A433">/*</>
<span color="#48A433">				 * Add two numbers.</>
<span color="#48A433">				 */</>
<span color="#4A9A9B">				public</> <span color="#4A9A9B">void</> Add(<span color="#4A9A9B">float</> value1, <span color="#4A9A9B">float</> value2)
				{
<span color="#48A433">					// Adding values:</>
<span color="#4A9A9B">					return</> value1 + value2;	
				}
			)text");

			MixTextPiece parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}
	};
}