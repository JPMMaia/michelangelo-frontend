#pragma once

#include "IRichTextMarkupWriter.h"
#include "Regex.h"

class FCodeMarkupWriter : public IRichTextMarkupWriter
{
public:
	static TSharedRef< FCodeMarkupWriter > Create();

public:
	virtual void Write(const TArray<FRichTextLine>& InLines, FString& Output) override;

private:
	static void EscapeText(FString& TextToEscape);

private:
	FCodeMarkupWriter() {}

private:
	TArray<FRichTextLine> BuildRuns(const TArray<FRichTextLine>& InLines);

private:
	//UCodeHighlighter CodeHighlighter;
};
