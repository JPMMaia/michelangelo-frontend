#include "Michelangelo.h"
#include "CodeMarkupWriter.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/TextHighlight/CSharpHighlighter.hpp"

TSharedRef<FCodeMarkupWriter> FCodeMarkupWriter::Create()
{
	return MakeShareable(new FCodeMarkupWriter());
}

void FCodeMarkupWriter::Write(const TArray<FRichTextLine>& InLines, FString& Output)
{
	FString fullText;

	// Concatenate every line into a single string:
	for (int32 LineIndex = 0; LineIndex < InLines.Num(); ++LineIndex)
	{
		const auto& Runs = InLines[LineIndex].Runs;
		for (int32 RunIndex = 0; RunIndex < Runs.Num(); ++RunIndex)
		{
			fullText.Append(Runs[RunIndex].Text);
		}
		if(LineIndex != InLines.Num() - 1)
			fullText.Append("\n");
	}

	// Parse and highlight code:
	TextHighlight::CSharpHighlighter highlighter(Common::Helpers::FStringToString(fullText));
	highlighter.Parse();
	
	Output = Common::Helpers::StringToFString(highlighter.ToString());
}
