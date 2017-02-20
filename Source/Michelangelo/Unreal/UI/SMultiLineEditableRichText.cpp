// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Michelangelo.h"

#include "Runtime/Slate/Private/Framework/Text/TextEditHelper.h"
#include "PlainTextLayoutMarshaller.h"
#include "SlateEditableTextLayout.h"
#include "ReflectionMetadata.h"
#include "IMenu.h"
#include "SMultiLineEditableRichText.h"

SMultiLineEditableRichText::SMultiLineEditableRichText()
	: bSelectAllTextWhenFocused(false)
	, bIsReadOnly(false)
	, AmountScrolledWhileRightMouseDown(0.0f)
	, bIsSoftwareCursor(false)
{
}

SMultiLineEditableRichText::~SMultiLineEditableRichText()
{
	// Needed to avoid "deletion of pointer to incomplete type 'FSlateEditableTextLayout'; no destructor called" error when using TUniquePtr
}

void SMultiLineEditableRichText::Construct(const FArguments& InArgs)
{
	bIsReadOnly = InArgs._IsReadOnly;

	OnTextChangedCallback = InArgs._OnTextChanged;
	OnTextCommittedCallback = InArgs._OnTextCommitted;
	OnCursorMovedCallback = InArgs._OnCursorMoved;
	bSelectAllTextWhenFocused = InArgs._SelectAllTextWhenFocused;
	bClearTextSelectionOnFocusLoss = InArgs._ClearTextSelectionOnFocusLoss;
	bClearKeyboardFocusOnCommit = InArgs._ClearKeyboardFocusOnCommit;
	bAllowContextMenu = InArgs._AllowContextMenu;
	OnContextMenuOpening = InArgs._OnContextMenuOpening;
	bRevertTextOnEscape = InArgs._RevertTextOnEscape;
	OnHScrollBarUserScrolled = InArgs._OnHScrollBarUserScrolled;
	OnVScrollBarUserScrolled = InArgs._OnVScrollBarUserScrolled;
	OnKeyDownHandler = InArgs._OnKeyDownHandler;
	ModiferKeyForNewLine = InArgs._ModiferKeyForNewLine;

	HScrollBar = InArgs._HScrollBar;
	if (HScrollBar.IsValid())
	{
		HScrollBar->SetUserVisibility(EVisibility::Collapsed);
		HScrollBar->SetOnUserScrolled(FOnUserScrolled::CreateSP(this, &SMultiLineEditableRichText::OnHScrollBarMoved));
	}

	VScrollBar = InArgs._VScrollBar;
	if (VScrollBar.IsValid())
	{
		VScrollBar->SetUserVisibility(EVisibility::Collapsed);
		VScrollBar->SetOnUserScrolled(FOnUserScrolled::CreateSP(this, &SMultiLineEditableRichText::OnVScrollBarMoved));
	}

	FTextBlockStyle TextStyle = *InArgs._TextStyle;
	if (InArgs._Font.IsSet() || InArgs._Font.IsBound())
	{
		TextStyle.SetFont(InArgs._Font.Get());
	}

	TSharedPtr<ITextLayoutMarshaller> Marshaller = InArgs._Marshaller;
	if (!Marshaller.IsValid())
	{
		Marshaller = FPlainTextLayoutMarshaller::Create();
	}

	EditableTextLayout = MakeUnique<FSlateEditableTextLayout>(*this, InArgs._Text, TextStyle, InArgs._TextShapingMethod, InArgs._TextFlowDirection, InArgs._CreateSlateTextLayout, Marshaller.ToSharedRef(), Marshaller.ToSharedRef());
	EditableTextLayout->SetHintText(InArgs._HintText);
	EditableTextLayout->SetTextWrapping(InArgs._WrapTextAt, InArgs._AutoWrapText, InArgs._WrappingPolicy);
	EditableTextLayout->SetMargin(InArgs._Margin);
	EditableTextLayout->SetJustification(InArgs._Justification);
	EditableTextLayout->SetLineHeightPercentage(InArgs._LineHeightPercentage);
	EditableTextLayout->SetDebugSourceInfo(TAttribute<FString>::Create(TAttribute<FString>::FGetter::CreateLambda([this] { return FReflectionMetaData::GetWidgetDebugInfo(this); })));

	// build context menu extender
	MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuExtension("EditText", EExtensionHook::Before, TSharedPtr<FUICommandList>(), InArgs._ContextMenuExtender);
}

void SMultiLineEditableRichText::SetText(const TAttribute< FText >& InText)
{
	EditableTextLayout->SetText(InText);
}

FText SMultiLineEditableRichText::GetText() const
{
	return EditableTextLayout->GetText();
}

FText SMultiLineEditableRichText::GetPlainText() const
{
	return EditableTextLayout->GetPlainText();
}

void SMultiLineEditableRichText::SetHintText(const TAttribute< FText >& InHintText)
{
	EditableTextLayout->SetHintText(InHintText);
}

FText SMultiLineEditableRichText::GetHintText() const
{
	return EditableTextLayout->GetHintText();
}

void SMultiLineEditableRichText::SetTextStyle(const FTextBlockStyle* InTextStyle)
{
	if (InTextStyle)
	{
		EditableTextLayout->SetTextStyle(*InTextStyle);
	}
	else
	{
		FArguments Defaults;
		check(Defaults._TextStyle);
		EditableTextLayout->SetTextStyle(*Defaults._TextStyle);
	}
}

void SMultiLineEditableRichText::SetFont(const TAttribute< FSlateFontInfo >& InNewFont)
{
	FTextBlockStyle TextStyle = EditableTextLayout->GetTextStyle();
	TextStyle.SetFont(InNewFont.Get());
	EditableTextLayout->SetTextStyle(TextStyle);
}

void SMultiLineEditableRichText::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	EditableTextLayout->SetTextShapingMethod(InTextShapingMethod);
}

void SMultiLineEditableRichText::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	EditableTextLayout->SetTextFlowDirection(InTextFlowDirection);
}

void SMultiLineEditableRichText::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	EditableTextLayout->SetWrapTextAt(InWrapTextAt);
}

void SMultiLineEditableRichText::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	EditableTextLayout->SetAutoWrapText(InAutoWrapText);
}

void SMultiLineEditableRichText::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	EditableTextLayout->SetWrappingPolicy(InWrappingPolicy);
}

void SMultiLineEditableRichText::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	EditableTextLayout->SetLineHeightPercentage(InLineHeightPercentage);
}

void SMultiLineEditableRichText::SetMargin(const TAttribute<FMargin>& InMargin)
{
	EditableTextLayout->SetMargin(InMargin);
}

void SMultiLineEditableRichText::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	EditableTextLayout->SetJustification(InJustification);
}

void SMultiLineEditableRichText::SetAllowContextMenu(const TAttribute< bool >& InAllowContextMenu)
{
	bAllowContextMenu = InAllowContextMenu;
}

void SMultiLineEditableRichText::OnHScrollBarMoved(const float InScrollOffsetFraction)
{
	EditableTextLayout->SetHorizontalScrollFraction(InScrollOffsetFraction);
	OnHScrollBarUserScrolled.ExecuteIfBound(InScrollOffsetFraction);
}

void SMultiLineEditableRichText::OnVScrollBarMoved(const float InScrollOffsetFraction)
{
	EditableTextLayout->SetVerticalScrollFraction(InScrollOffsetFraction);
	OnVScrollBarUserScrolled.ExecuteIfBound(InScrollOffsetFraction);
}

bool SMultiLineEditableRichText::IsTextReadOnly() const
{
	return bIsReadOnly.Get(false);
}

bool SMultiLineEditableRichText::IsTextPassword() const
{
	return false;
}

bool SMultiLineEditableRichText::IsMultiLineTextEdit() const
{
	return true;
}

bool SMultiLineEditableRichText::ShouldJumpCursorToEndWhenFocused() const
{
	return false;
}

bool SMultiLineEditableRichText::ShouldSelectAllTextWhenFocused() const
{
	return bSelectAllTextWhenFocused.Get(false);
}

bool SMultiLineEditableRichText::ShouldClearTextSelectionOnFocusLoss() const
{
	return bClearTextSelectionOnFocusLoss.Get(false);
}

bool SMultiLineEditableRichText::ShouldRevertTextOnEscape() const
{
	return bRevertTextOnEscape.Get(false);
}

bool SMultiLineEditableRichText::ShouldClearKeyboardFocusOnCommit() const
{
	return bClearKeyboardFocusOnCommit.Get(false);
}

bool SMultiLineEditableRichText::ShouldSelectAllTextOnCommit() const
{
	return false;
}

bool SMultiLineEditableRichText::CanInsertCarriageReturn() const
{
	return FSlateApplication::Get().GetModifierKeys().AreModifersDown(ModiferKeyForNewLine);
}

bool SMultiLineEditableRichText::CanTypeCharacter(const TCHAR InChar) const
{
	return true;
}

void SMultiLineEditableRichText::EnsureActiveTick()
{
	TSharedPtr<FActiveTimerHandle> ActiveTickTimerPin = ActiveTickTimer.Pin();
	if (ActiveTickTimerPin.IsValid())
	{
		return;
	}

	auto DoActiveTick = [this](double InCurrentTime, float InDeltaTime) -> EActiveTimerReturnType
	{
		// Continue if we still have focus, otherwise treat as a fire-and-forget Tick() request
		const bool bShouldAppearFocused = HasKeyboardFocus() || EditableTextLayout->HasActiveContextMenu();
		return (bShouldAppearFocused) ? EActiveTimerReturnType::Continue : EActiveTimerReturnType::Stop;
	};

	const float TickPeriod = EditableTextDefs::BlinksPerSecond * 0.5f;
	ActiveTickTimer = RegisterActiveTimer(TickPeriod, FWidgetActiveTimerDelegate::CreateLambda(DoActiveTick));
}

EKeyboardType SMultiLineEditableRichText::GetVirtualKeyboardType() const
{
	return Keyboard_Default;
}

TSharedRef<SWidget> SMultiLineEditableRichText::GetSlateWidget()
{
	return SharedThis(this);
}

TSharedPtr<SWidget> SMultiLineEditableRichText::BuildContextMenuContent() const
{
	if (!bAllowContextMenu.Get())
	{
		return nullptr;
	}

	if (OnContextMenuOpening.IsBound())
	{
		return OnContextMenuOpening.Execute();
	}

	return EditableTextLayout->BuildDefaultContextMenu(MenuExtender);
}

void SMultiLineEditableRichText::OnTextChanged(const FText& InText)
{
	OnTextChangedCallback.ExecuteIfBound(InText);
}

void SMultiLineEditableRichText::OnTextCommitted(const FText& InText, const ETextCommit::Type InTextAction)
{
	OnTextCommittedCallback.ExecuteIfBound(InText, InTextAction);
}

void SMultiLineEditableRichText::OnCursorMoved(const FTextLocation& InLocation)
{
	OnCursorMovedCallback.ExecuteIfBound(InLocation);
}

float SMultiLineEditableRichText::UpdateAndClampHorizontalScrollBar(const float InViewOffset, const float InViewFraction, const EVisibility InVisiblityOverride)
{
	if (HScrollBar.IsValid())
	{
		HScrollBar->SetState(InViewOffset, InViewFraction);
		HScrollBar->SetUserVisibility(InVisiblityOverride);
		if (!HScrollBar->IsNeeded())
		{
			// We cannot scroll, so ensure that there is no offset
			return 0.0f;
		}
	}

	return EditableTextLayout->GetScrollOffset().X;
}

float SMultiLineEditableRichText::UpdateAndClampVerticalScrollBar(const float InViewOffset, const float InViewFraction, const EVisibility InVisiblityOverride)
{
	if (VScrollBar.IsValid())
	{
		VScrollBar->SetState(InViewOffset, InViewFraction);
		VScrollBar->SetUserVisibility(InVisiblityOverride);
		if (!VScrollBar->IsNeeded())
		{
			// We cannot scroll, so ensure that there is no offset
			return 0.0f;
		}
	}

	return EditableTextLayout->GetScrollOffset().Y;
}

FReply SMultiLineEditableRichText::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	EditableTextLayout->HandleFocusReceived(InFocusEvent);
	return FReply::Handled();
}

void SMultiLineEditableRichText::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	bIsSoftwareCursor = false;
	EditableTextLayout->HandleFocusLost(InFocusEvent);
}

bool SMultiLineEditableRichText::AnyTextSelected() const
{
	return EditableTextLayout->AnyTextSelected();
}

void SMultiLineEditableRichText::SelectAllText()
{
	EditableTextLayout->SelectAllText();
}

void SMultiLineEditableRichText::ClearSelection()
{
	EditableTextLayout->ClearSelection();
}

FText SMultiLineEditableRichText::GetSelectedText() const
{
	return EditableTextLayout->GetSelectedText();
}

void SMultiLineEditableRichText::InsertTextAtCursor(const FText& InText)
{
	EditableTextLayout->InsertTextAtCursor(InText.ToString());
}

void SMultiLineEditableRichText::InsertTextAtCursor(const FString& InString)
{
	EditableTextLayout->InsertTextAtCursor(InString);
}

void SMultiLineEditableRichText::InsertRunAtCursor(TSharedRef<IRun> InRun)
{
	EditableTextLayout->InsertRunAtCursor(InRun);
}

void SMultiLineEditableRichText::GoTo(const FTextLocation& NewLocation)
{
	EditableTextLayout->GoTo(NewLocation);
}

void SMultiLineEditableRichText::GoTo(ETextLocation GoToLocation)
{
	EditableTextLayout->GoTo(GoToLocation);
}

void SMultiLineEditableRichText::ScrollTo(const FTextLocation& NewLocation)
{
	EditableTextLayout->ScrollTo(NewLocation);
}

void SMultiLineEditableRichText::ApplyToSelection(const FRunInfo& InRunInfo, const FTextBlockStyle& InStyle)
{
	EditableTextLayout->ApplyToSelection(InRunInfo, InStyle);
}

TSharedPtr<const IRun> SMultiLineEditableRichText::GetRunUnderCursor() const
{
	return EditableTextLayout->GetRunUnderCursor();
}

TArray<TSharedRef<const IRun>> SMultiLineEditableRichText::GetSelectedRuns() const
{
	return EditableTextLayout->GetSelectedRuns();
}

TSharedPtr<const SScrollBar> SMultiLineEditableRichText::GetHScrollBar() const
{
	return HScrollBar;
}

TSharedPtr<const SScrollBar> SMultiLineEditableRichText::GetVScrollBar() const
{
	return VScrollBar;
}

void SMultiLineEditableRichText::Refresh()
{
	EditableTextLayout->Refresh();
}

void SMultiLineEditableRichText::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	EditableTextLayout->Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

int32 SMultiLineEditableRichText::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = EditableTextLayout->OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, ShouldBeEnabled(bParentEnabled));

	if (bIsSoftwareCursor)
	{
		const FSlateBrush* Brush = FCoreStyle::Get().GetBrush(TEXT("SoftwareCursor_Grab"));

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			++LayerId,
			AllottedGeometry.ToPaintGeometry(SoftwareCursorPosition - (Brush->ImageSize / 2), Brush->ImageSize),
			Brush,
			MyClippingRect
		);
	}

	return LayerId;
}

void SMultiLineEditableRichText::CacheDesiredSize(float LayoutScaleMultiplier)
{
	EditableTextLayout->CacheDesiredSize(LayoutScaleMultiplier);
	SWidget::CacheDesiredSize(LayoutScaleMultiplier);
}

FVector2D SMultiLineEditableRichText::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return EditableTextLayout->ComputeDesiredSize(LayoutScaleMultiplier);
}

FChildren* SMultiLineEditableRichText::GetChildren()
{
	return EditableTextLayout->GetChildren();
}

void SMultiLineEditableRichText::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	EditableTextLayout->OnArrangeChildren(AllottedGeometry, ArrangedChildren);
}

bool SMultiLineEditableRichText::SupportsKeyboardFocus() const
{
	return true;
}

FReply SMultiLineEditableRichText::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent)
{
	return EditableTextLayout->HandleKeyChar(InCharacterEvent);
}

FReply SMultiLineEditableRichText::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	// First call the user defined key handler, there might be overrides to normal functionality
	if (OnKeyDownHandler.IsBound())
	{
		Reply = OnKeyDownHandler.Execute(MyGeometry, InKeyEvent);
	}

	if (!Reply.IsEventHandled())
	{
		Reply = EditableTextLayout->HandleKeyDown(InKeyEvent);
	}

	return Reply;
}

FReply SMultiLineEditableRichText::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	return FReply::Unhandled();
}

FReply SMultiLineEditableRichText::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		AmountScrolledWhileRightMouseDown = 0.0f;
	}

	return EditableTextLayout->HandleMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SMultiLineEditableRichText::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bool bWasRightClickScrolling = IsRightClickScrolling();
		AmountScrolledWhileRightMouseDown = 0.0f;

		if (bWasRightClickScrolling)
		{
			bIsSoftwareCursor = false;
			const FVector2D CursorPosition = MyGeometry.LocalToAbsolute(SoftwareCursorPosition);
			const FIntPoint OriginalMousePos(CursorPosition.X, CursorPosition.Y);
			return FReply::Handled().ReleaseMouseCapture().SetMousePos(OriginalMousePos);
		}
	}

	return EditableTextLayout->HandleMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SMultiLineEditableRichText::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		const float ScrollByAmount = MouseEvent.GetCursorDelta().Y / MyGeometry.Scale;

		// If scrolling with the right mouse button, we need to remember how much we scrolled.
		// If we did not scroll at all, we will bring up the context menu when the mouse is released.
		AmountScrolledWhileRightMouseDown += FMath::Abs(ScrollByAmount);

		if (IsRightClickScrolling())
		{
			const FVector2D PreviousScrollOffset = EditableTextLayout->GetScrollOffset();

			FVector2D NewScrollOffset = PreviousScrollOffset;
			NewScrollOffset.Y -= ScrollByAmount;
			EditableTextLayout->SetScrollOffset(NewScrollOffset, MyGeometry);

			if (!bIsSoftwareCursor)
			{
				SoftwareCursorPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
				bIsSoftwareCursor = true;
			}

			if (PreviousScrollOffset.Y != NewScrollOffset.Y)
			{
				const float ScrollMax = EditableTextLayout->GetSize().Y - MyGeometry.Size.Y;
				const float ScrollbarOffset = (ScrollMax != 0.0f) ? NewScrollOffset.Y / ScrollMax : 0.0f;
				OnVScrollBarUserScrolled.ExecuteIfBound(ScrollbarOffset);
				SoftwareCursorPosition.Y += (PreviousScrollOffset.Y - NewScrollOffset.Y);
			}

			return FReply::Handled().UseHighPrecisionMouseMovement(AsShared());
		}
	}

	return EditableTextLayout->HandleMouseMove(MyGeometry, MouseEvent);
}

FReply SMultiLineEditableRichText::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (VScrollBar.IsValid() && VScrollBar->IsNeeded())
	{
		const float ScrollAmount = -MouseEvent.GetWheelDelta() * WheelScrollAmount;

		const FVector2D PreviousScrollOffset = EditableTextLayout->GetScrollOffset();

		FVector2D NewScrollOffset = PreviousScrollOffset;
		NewScrollOffset.Y += ScrollAmount;
		EditableTextLayout->SetScrollOffset(NewScrollOffset, MyGeometry);

		if (PreviousScrollOffset.Y != NewScrollOffset.Y)
		{
			const float ScrollMax = EditableTextLayout->GetSize().Y - MyGeometry.Size.Y;
			const float ScrollbarOffset = (ScrollMax != 0.0f) ? NewScrollOffset.Y / ScrollMax : 0.0f;
			OnVScrollBarUserScrolled.ExecuteIfBound(ScrollbarOffset);
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

FReply SMultiLineEditableRichText::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return EditableTextLayout->HandleMouseButtonDoubleClick(MyGeometry, MouseEvent);
}

FCursorReply SMultiLineEditableRichText::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (IsRightClickScrolling() && CursorEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		return FCursorReply::Cursor(EMouseCursor::None);
	}
	else
	{
		return FCursorReply::Cursor(EMouseCursor::TextEditBeam);
	}
}

bool SMultiLineEditableRichText::IsInteractable() const
{
	return IsEnabled();
}

bool SMultiLineEditableRichText::IsRightClickScrolling() const
{
	return AmountScrolledWhileRightMouseDown >= FSlateApplication::Get().GetDragTriggerDistance() && VScrollBar.IsValid() && VScrollBar->IsNeeded();
}

