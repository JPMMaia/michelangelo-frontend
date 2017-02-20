#include "Michelangelo.h"
#include "CodeBox.h"

#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

UCodeBox::UCodeBox(const FObjectInitializer& ObjectInitializer) :
	UMultiLineEditableTextBox(ObjectInitializer)
{
	OnTextChanged.AddDynamic(this, &UCodeBox::OnCodeChanged);
}

void UCodeBox::OnCodeChanged(const FText& code)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Committed!")));
}
