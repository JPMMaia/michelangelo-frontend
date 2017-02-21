#include "Michelangelo.h"
#include "CodeHighlighter.h"
#include "NonUnreal/nlohmann/JSON/json.hpp"
#include "Unreal/Common/UnrealHelpers.h"

#include <regex>
#include <fstream>

using namespace Common;
using namespace std;

UCodeHighlighter::UCodeHighlighter() :
	RegexHelper(L"Highlighting.json")
{
}

TArray<IRichTextMarkupWriter::FRichTextLine> UCodeHighlighter::Parse(const TArray<IRichTextMarkupWriter::FRichTextLine>& Lines)
{
	TArray<IRichTextMarkupWriter::FRichTextLine> Output;
	Output.AddDefaulted(Lines.Num());

	ParseComments(Lines, Output);

	return Output;
}

void UCodeHighlighter::ParseComments(const TArray<IRichTextMarkupWriter::FRichTextLine>& OriginalLines, TArray<IRichTextMarkupWriter::FRichTextLine>& OutputLines)
{
	for (int32 LineIndex = 0; LineIndex < OriginalLines.Num(); ++LineIndex)
	{
		auto& OutputLine = OutputLines[LineIndex];

		auto Text = OriginalLines[LineIndex].Runs[0].Text;

		FRunInfo RunInfo;
		if (Text.StartsWith("//"))
		{
			RunInfo.Name = TEXT("Span");
			RunInfo.MetaData.Add(TEXT("Color"), TEXT("#ff00ff"));
		}

		OutputLine.Runs.Add(IRichTextMarkupWriter::FRichTextRun(RunInfo, Text));
	}
}

