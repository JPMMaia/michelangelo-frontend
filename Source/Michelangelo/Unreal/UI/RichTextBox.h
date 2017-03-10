// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/UMG/Public/UMGStyle.h>
#include <Runtime/UMG/Public/Components/RichTextBlockDecorator.h>
#include <Runtime/UMG/Public/Components/TextWidgetTypes.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include "SMultiLineEditableRichText.h"
#include "RichTextBox.generated.h"

/* Replace the project API with yours! */
UCLASS()
class URichTextBox : public UTextLayoutWidget
{
	GENERATED_BODY()

public:
	explicit URichTextBox(const FObjectInitializer& ObjectInitializer);

public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "GetText (Multi-Line Text Box)"))
		FText GetText() const;

	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "GetPlainText (Multi-Line Text Box)"))
		FText GetPlainText() const;

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "SetText (Multi-Line Text Box)"))
		void SetText(FText InText);

	UFUNCTION(BlueprintCallable, Category = "Widget", meta = (DisplayName = "SetError (Multi-Line Text Box)"))
		void SetError(FText InError);

public:

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget

protected:
	/** The text to display */
	UPROPERTY(EditAnywhere, Category = Content, meta = (MultiLine = "true"))
		FText Text;

	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
		FGetText TextDelegate;

	/** The default color for the text. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FLinearColor ColorAndOpacity;

	/** The default font for the text. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FSlateFontInfo Font;

	/** The direction the shadow is cast */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FVector2D ShadowOffset;

	/** The color of the shadow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (DisplayName = "Shadow Color"))
		FLinearColor ShadowColorAndOpacity;

	UPROPERTY(EditAnywhere, Instanced, Category = Decorators)
		TArray<class URichTextBlockDecorator*> Decorators;

protected:
	FTextBlockStyle DefaultStyle;

	/** Native Slate Widget */
	TSharedPtr<SMultiLineEditableRichText> MyRichTextBlock;

	FText PlainText;
};