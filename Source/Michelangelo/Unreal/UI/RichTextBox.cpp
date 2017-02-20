// Fill out your copyright notice in the Description page of Project Settings.
#include "Michelangelo.h"
#include "RichTextBox.h"
#include "Runtime/UMG/Public/Components/RichTextBlockDecorator.h"

#define LOCTEXT_NAMESPACE "UMG"

URichTextBox::URichTextBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
	Font = FSlateFontInfo(RobotoFontObj.Object, 12, FName("Regular"));
	ColorAndOpacity = FLinearColor::White;
	LineHeightPercentage = 1;
	ShadowOffset = FVector2D(1.0f, 1.0f);
	ShadowColorAndOpacity = FLinearColor::Transparent;


	Decorators.Add(ObjectInitializer.CreateOptionalDefaultSubobject<URichTextBlockDecorator>(this, FName("DefaultDecorator")));
}

void URichTextBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRichTextBlock.Reset();
}

void URichTextBox::SetText(FText InText)
{
	Text = InText;
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetText(Text);
	}
}

FText URichTextBox::GetText()
{
	return Text;
}

TSharedRef<SWidget> URichTextBox::RebuildWidget()
{
	//+ OnHyperlinkClicked = FSlateHyperlinkRun::FOnClick::CreateStatic(&RichTextHelper::OnBrowserLinkClicked, AsShared());
	//+ FHyperlinkDecorator::Create(TEXT("browser"), OnHyperlinkClicked))
	//+MakeShareable(new FDefaultRichTextDecorator(Font, Color));

	DefaultStyle.SetFont(Font);
	DefaultStyle.SetColorAndOpacity(ColorAndOpacity);
	DefaultStyle.SetShadowColorAndOpacity(ShadowColorAndOpacity);
	DefaultStyle.SetShadowOffset(ShadowOffset);

	TArray< TSharedRef< class ITextDecorator > > CreatedDecorators;

	for (URichTextBlockDecorator* Decorator : Decorators)
	{
		if (Decorator)
		{
			CreatedDecorators.Add(Decorator->CreateDecorator(Font, ColorAndOpacity));
		}
	}

	MyRichTextBlock =
		SNew(SRichTextBlock)
		.Justification(Justification)
		.AutoWrapText(AutoWrapText)
		.WrapTextAt(WrapTextAt)
		.Margin(Margin)
		.LineHeightPercentage(LineHeightPercentage)
		.TextStyle(&DefaultStyle)
		.Decorators(CreatedDecorators);

	return MyRichTextBlock.ToSharedRef();
}

void URichTextBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UE_LOG(LogWindows, Log, TEXT("Synchronizing Properties"));

	TAttribute<FText> TextBinding = OPTIONAL_BINDING(FText, Text);

	DefaultStyle.SetFont(Font);
	DefaultStyle.SetColorAndOpacity(ColorAndOpacity);
	DefaultStyle.SetShadowColorAndOpacity(ShadowColorAndOpacity);
	DefaultStyle.SetShadowOffset(ShadowOffset);

	MyRichTextBlock->SetText(TextBinding);

}

#if WITH_EDITOR

// const FSlateBrush* URichTextBox::GetEditorIcon()
// {
// 	return FUMGStyle::Get().GetBrush("Widget.RichTextBlock");
// }

const FText URichTextBox::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE