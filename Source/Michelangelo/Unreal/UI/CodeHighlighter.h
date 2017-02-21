#pragma once

#include "NonUnreal/Common/CSharpRegexHelper.hpp"
#include "IRichTextMarkupWriter.h"
#include "Runtime/Slate/Public/Framework/Text/IRichTextMarkupWriter.h"
#include "CodeHighlighter.generated.h"

UCLASS()
class UCodeHighlighter : public UObject
{
	GENERATED_BODY()

public:
	UCodeHighlighter();

	TArray<IRichTextMarkupWriter::FRichTextLine> Parse(const TArray<IRichTextMarkupWriter::FRichTextLine>& Lines);

private:
	void ParseComments(const TArray<IRichTextMarkupWriter::FRichTextLine>& OriginalLines, TArray<IRichTextMarkupWriter::FRichTextLine>& OutputLines);

private:
	CSharpRegexHelper RegexHelper;
};
