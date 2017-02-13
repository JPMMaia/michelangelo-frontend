#include "Michelangelo.h"
#include "StandardWidget.h"
#include "StandardHUD.h"

#define LOCTEXT_NAMESPACE "SStandardWidget"

void SStandardWidget::Construct(const FArguments& InArgs)
{
	m_ownerHUD = InArgs._m_ownerHUD;

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
		.ColorAndOpacity(FLinearColor::Red)
		.ShadowOffset(FIntPoint(-1, 1))
		.Font(FSlateFontInfo("Veranda", 16))
		.Text(LOCTEXT("HelloSlate", "Hello, Slate!"))
		]
		];
}

#undef LOCTEXT_NAMESPACE