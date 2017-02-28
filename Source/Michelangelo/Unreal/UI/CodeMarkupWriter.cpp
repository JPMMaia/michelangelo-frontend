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

void FCodeMarkupWriter::EscapeText(FString& TextToEscape)
{
	// List of characters that we have to escape to avoid accidental rich-text formatting
	static const TPair<TCHAR, FString> EscapeCharacters[] = {
		TPairInitializer<TCHAR, FString>('&', "&amp;"),
		TPairInitializer<TCHAR, FString>('"', "&quot;"),
		TPairInitializer<TCHAR, FString>('<', "&lt;"),
		TPairInitializer<TCHAR, FString>('>', "&gt;"),
	};

	// First count the extra space needed
	int32 EscapedStringLen = TextToEscape.Len();
	for (int32 TextIndex = 0; TextIndex < TextToEscape.Len(); ++TextIndex)
	{
		const TCHAR CharToEscape = TextToEscape[TextIndex];
		for (const auto& EscapeCharacter : EscapeCharacters)
		{
			if (CharToEscape == EscapeCharacter.Key)
			{
				EscapedStringLen += (EscapeCharacter.Value.Len() - 1);
				break;
			}
		}
	}

	if (EscapedStringLen == TextToEscape.Len())
	{
		return;
	}

	TextToEscape.Reserve(EscapedStringLen);

	// Then perform the actual escape (backwards to make iteration simpler)
	for (int32 TextIndex = TextToEscape.Len() - 1; TextIndex >= 0; --TextIndex)
	{
		const TCHAR CharToEscape = TextToEscape[TextIndex];
		for (const auto& EscapeCharacter : EscapeCharacters)
		{
			if (CharToEscape == EscapeCharacter.Key)
			{
				TextToEscape.RemoveAt(TextIndex, 1, false);
				TextToEscape.InsertAt(TextIndex, EscapeCharacter.Value);
				break;
			}
		}
	}
}
