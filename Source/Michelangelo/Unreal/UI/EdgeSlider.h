#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/UMG/Public/UMGStyle.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/Button.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <Runtime/Core/Public/Delegates/Delegate.h>
#include <Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "SlidePosition.h"
#include "ResizableBorder.h"
#include "EdgeSlider.generated.h"

class UDragDropOperation;

UCLASS()
class UEdgeSlider : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour", meta = (DisplayName = "Slide Position"))
	ESlidePosition SlidePosition;

public:
	explicit UEdgeSlider(const FObjectInitializer& initializer);

public:
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};