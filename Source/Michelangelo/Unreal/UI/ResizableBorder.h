#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/UMG/Public/UMGStyle.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/Border.h>
#include <Runtime/Core/Public/Delegates/Delegate.h>
#include <Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "SlidePosition.h"
#include "ResizableBorder.generated.h"

UCLASS()
class UResizableBorder : public UBorder
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void ResizeAccordingToMousePosition(const FGeometry& geometry, const FPointerEvent& mouseEvent);

	UFUNCTION()
	FVector2D GetMouseLocalPosition(const FGeometry& geometry, const FPointerEvent& mouseEvent) const;
};