#include "Michelangelo.h"
#include "CodeMarkupWriter.h"

TSharedRef< FCodeMarkupWriter > FCodeMarkupWriter::Create()
{
	return MakeShareable(new FCodeMarkupWriter());
}

void FCodeMarkupWriter::Write(const TArray<FRichTextLine>& InLines, FString& Output)
{
	auto Lines = BuildRuns(InLines);

	for (int32 LineIndex = 0; LineIndex < Lines.Num(); ++LineIndex)
	{
		const FRichTextLine& Line = Lines[LineIndex];

		// Append \n to the end of the previous line
		if (LineIndex > 0)
		{
			Output.AppendChar('\n');
		}

		for (const FRichTextRun& Run : Line.Runs)
		{
			// Our rich-text format takes the form of <Name metakey1="metavalue1" metakey2="metavalue2">The Text</>
			const bool bHasTag = !Run.Info.Name.IsEmpty();
			if (bHasTag)
			{
				Output.AppendChar('<');

				Output.Append(Run.Info.Name);

				for (const TPair<FString, FString>& MetaDataEntry : Run.Info.MetaData)
				{
					Output.AppendChar(' ');
					Output.Append(MetaDataEntry.Key);
					Output.AppendChar('=');
					Output.AppendChar('"');
					Output.Append(MetaDataEntry.Value);
					Output.AppendChar('"');
				}

				Output.AppendChar('>');
			}

			FString RunText = Run.Text;
			EscapeText(RunText);
			Output.Append(RunText);

			if (bHasTag)
			{
				Output.Append(TEXT("</>"));
			}
		}
	}
}

TArray<IRichTextMarkupWriter::FRichTextLine> FCodeMarkupWriter::BuildRuns(const TArray<FRichTextLine>& InLines)
{
	TArray<IRichTextMarkupWriter::FRichTextLine> Output;
	Output.AddDefaulted(InLines.Num());

	for (int32 LineIndex = 0; LineIndex < Output.Num(); ++LineIndex)
	{
		auto& Line = Output[LineIndex];

		auto Text = InLines[LineIndex].Runs[0].Text;
		
		FRunInfo RunInfo;
		if(Text.StartsWith("//"))
		{
			RunInfo.Name = TEXT("Span");
			RunInfo.MetaData.Add(TEXT("Color"), TEXT("#ff00ff"));
		}

		Line.Runs.Add(FRichTextRun(RunInfo, Text));
	}

	return Output;
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
