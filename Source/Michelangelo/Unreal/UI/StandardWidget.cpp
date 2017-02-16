#include "Michelangelo.h"
#include "StandardWidget.h"
#include "StandardHUD.h"

#include <Runtime/Slate/Public/Widgets/Docking/SDockTab.h>

#define LOCTEXT_NAMESPACE "SStandardWidget"

void SStandardWidget::Construct(const FArguments& InArgs)
{
	m_ownerHUD = InArgs._m_ownerHUD;

	/*ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			/*SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().HAlign(HAlign_Fill)
			[
				SNew(STextBlock).Text(FText::FromString("Tutorial Name"))
			]
			+ SHorizontalBox::Slot().HAlign(HAlign_Center)
			[
				SNew(SComboButton)
				.ButtonContent()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("BlockingVolumeMenu", "Create Blocking Volume"))
				]
				//.OnGetMenuContent(this, &SStandardWidget::OnGetGrammarType)
			]
			+ SHorizontalBox::Slot().HAlign(HAlign_Right)
			[
				SNew(SButton)
				.Text(FText::FromString("GO"))
				.OnClicked(this, &SStandardWidget::OnGoButtonClicked)
			]*/
		/*];*/
}

TSharedRef<SWidget> SStandardWidget::OnGetGrammarType()
{
	return SNew(STextBlock);
}
FReply SStandardWidget::OnGoButtonClicked()
{
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
