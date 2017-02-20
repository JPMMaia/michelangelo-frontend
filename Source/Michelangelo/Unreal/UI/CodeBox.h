#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/Core/Public/Delegates/Delegate.h>
#include <Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include <Runtime/UMG/Public/Components/MultiLineEditableTextBox.h>

#include "RichTextBox.h"
#include "CodeBox.generated.h"

UCLASS()
class UCodeBox : public UMultiLineEditableTextBox
{
	GENERATED_BODY()

public:
	explicit UCodeBox(const FObjectInitializer& Initializer);

public:

	UFUNCTION()
	void OnCodeChanged(const FText& code);
};