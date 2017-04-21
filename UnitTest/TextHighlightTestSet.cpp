#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TextHighlight;

namespace UnitTest
{
	TEST_CLASS(TextHighlightTestSet)
	{
	private:
		std::string m_commentsColor;
		std::string m_stringsColor;
		std::string m_keywordsColor;

	public:

		TEST_METHOD_INITIALIZE(TextHighlightTestSetInitialize)
		{
			auto settings = CSharpHighlightSettings::Get();
			m_commentsColor = settings->GetColor("comments");
			m_stringsColor = settings->GetColor("strings");
			m_keywordsColor = settings->GetColor("keywords");
		}

		TEST_METHOD(TextHighlightTestOneLineComment)
		{
			std::string input("// A comment");
			std::string expectedOutput("<span color=\"#" + m_commentsColor + "\">// A comment</>");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestMultiLine)
		{
			auto settings = CSharpHighlightSettings::Get();

			std::string input("/* Multi\nLine\nComment\n*/\n");
			std::string expectedOutput("<span color=\"#" + m_commentsColor + "\">/* Multi</>\n<span color=\"#" + m_commentsColor + "\">Line</>\n<span color=\"#" + m_commentsColor + "\">Comment</>\n<span color=\"#" + m_commentsColor + "\">*/</>\n");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestString1)
		{
			std::string input("\"A Quote Bip\"");
			std::string expectedOutput("<span color=\"#" + m_stringsColor + "\">&quot;A Quote Bip&quot;</>");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestString2)
		{
			std::string input("\" \\n \n Hello!");
			std::string expectedOutput("<span color=\"#" + m_stringsColor + "\">&quot; \\n </>\n Hello!");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestKeywords)
		{
			std::string input("public void Add(float value1, float value2)");
			std::string expectedOutput("<span color=\"#" + m_keywordsColor + "\">public</> <span color=\"#" + m_keywordsColor + "\">void</> Add(<span color=\"#" + m_keywordsColor + "\">float</> value1, <span color=\"#" + m_keywordsColor + "\">float</> value2)");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestMix1)
		{
			std::string input("var value = 0.0f; // Declaring var");
			std::string expectedOutput("<span color=\"#" + m_keywordsColor + "\">var</> value = 0.0f; <span color=\"#" + m_commentsColor + "\">// Declaring var</>");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestMix2)
		{
			std::string input("var value1 = 0.0f; /* Declaring\nvar\n*/\nvar value2 = 0.0f;\n");
			std::string expectedOutput("<span color=\"#" + m_keywordsColor + "\">var</> value1 = 0.0f; <span color=\"#" + m_commentsColor + "\">/* Declaring</>\n<span color=\"#" + m_commentsColor + "\">var</>\n<span color=\"#" + m_commentsColor + "\">*/</>\n<span color=\"#" + m_keywordsColor + "\">var</> value2 = 0.0f;\n");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}

		TEST_METHOD(TextHighlightTestMix3)
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
				<span color="#)text" + m_commentsColor + R"t(">/*</>
<span color="#)t" + m_commentsColor + R"t(">				 * Add two numbers.</>
<span color="#)t" + m_commentsColor + R"t(">				 */</>
				<span color="#)t" + m_keywordsColor + R"t(">public</> <span color="#)t" + m_keywordsColor + R"t(">void</> Add(<span color="#)t" + m_keywordsColor + R"t(">float</> value1, <span color="#)t" + m_keywordsColor + R"t(">float</> value2)
				{
					<span color="#)t" + m_commentsColor + R"t(">// Adding values:</>
					<span color="#)t" + m_keywordsColor + R"t(">return</> value1 + value2;	
				}
			)t");

			CSharpHighlighter parser(input);
			parser.Parse();
			Assert::AreEqual(expectedOutput, parser.ToString());
		}
	};
}