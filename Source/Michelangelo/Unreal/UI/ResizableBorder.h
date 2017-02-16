#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/UMG/Public/UMGStyle.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/Border.h>
#include <Runtime/Core/Public/Delegates/Delegate.h>
#include <Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "ResizableBorder.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBorderDraggingEvent, const FPointerEvent&, MouseEvent);

UCLASS()
class UResizableBorder : public UBorder
{
	GENERATED_BODY()

public:
	explicit UResizableBorder(const FObjectInitializer& initializer);

	UFUNCTION()
	FEventReply OnMouseButtonDown(FGeometry geometry, const FPointerEvent& mouseEvent);

	UFUNCTION()
	FEventReply OnMouseButtonUp(FGeometry geometry, const FPointerEvent& mouseEvent);

	UFUNCTION()
	FEventReply OnMouseMove(FGeometry geometry, const FPointerEvent& mouseEvent);

public:

	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	FOnBorderDraggingEvent OnBorderDraggingEvent;

private:

	UFUNCTION()
	void ResizeAccordingToMousePosition(const FGeometry& geometry, const FPointerEvent& mouseEvent);

private:
	bool m_mouseButtonDown;
};