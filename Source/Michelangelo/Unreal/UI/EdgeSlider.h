#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/UMG/Public/UMGStyle.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/Button.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <Runtime/Core/Public/Delegates/Delegate.h>
#include <Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "EdgeSlider.generated.h"

class UDragDropOperation;

UCLASS()
class UEdgeSlider : public UUserWidget
{
	GENERATED_BODY()

public:
	explicit UEdgeSlider(const FObjectInitializer& initializer);

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void OnDragReleased();

private:
	bool m_focus;
	FVector2D m_lastMousePosition;
};