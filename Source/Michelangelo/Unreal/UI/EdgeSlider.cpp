#include "Michelangelo.h"
#include "EdgeSlider.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/UMG/Public/Blueprint/DragDropOperation.h>
#include "ResizableBorder.h"
#include <thread>

UEdgeSlider::UEdgeSlider(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer),
	SlidePosition(ESlidePosition::Right),
	m_isPressed(false)
{
}

FReply UEdgeSlider::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//auto mousePosition = InMouseEvent.GetScreenSpacePosition();
	//mousePosition = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, mousePosition);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Mouse Move, %d, %d"), static_cast<int>(mousePosition.X), static_cast<int>(mousePosition.Y)));

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		auto parent = GetParent();
		if (parent)
		{
			auto grandParent = parent->GetParent();
			if (grandParent)
			{
				auto resizableWidget = Cast<UResizableBorder>(grandParent);
				resizableWidget->ResizeAccordingToMousePosition(InGeometry, InMouseEvent);
			}
		}
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}
