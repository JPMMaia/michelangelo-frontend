#include "Michelangelo.h"
#include "ResizableBorder.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

UResizableBorder::UResizableBorder(const FObjectInitializer& ObjectInitializer) :
	UBorder(),
	m_mouseButtonDown(false)
{
	OnMouseButtonDownEvent.BindDynamic(this, &UResizableBorder::OnMouseButtonDown);
	OnMouseButtonUpEvent.BindDynamic(this, &UResizableBorder::OnMouseButtonUp);
	OnMouseMoveEvent.BindDynamic(this, &UResizableBorder::OnMouseMove);
}

FEventReply UResizableBorder::OnMouseButtonDown(FGeometry geometry, const FPointerEvent& mouseEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Down")));

	m_mouseButtonDown = true;

	// Detect if a user starts dragging in this widget later, if the left mouse button is clicked:
	auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton);

	return UWidgetBlueprintLibrary::CaptureMouse(detectDragReply, this);
}

FEventReply UResizableBorder::OnMouseButtonUp(FGeometry geometry, const FPointerEvent& mouseEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Up")));

	m_mouseButtonDown = false;

	// Detect if a user starts dragging in this widget later, if there is mouse movement:
	auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::MouseX);

	return UWidgetBlueprintLibrary::ReleaseMouseCapture(detectDragReply);
}

FEventReply UResizableBorder::OnMouseMove(FGeometry geometry, const FPointerEvent& mouseEvent)
{
	if (!m_mouseButtonDown)
		return FEventReply(true);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Move")));

	// Detect if a user starts dragging in this widget later, if the left mouse button is clicked:
	auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton);

	// Resize widget according to the mouse position:
	ResizeAccordingToMousePosition(geometry, mouseEvent);
	
	return UWidgetBlueprintLibrary::CaptureMouse(detectDragReply, this);
}

void UResizableBorder::ResizeAccordingToMousePosition(const FGeometry& geometry, const FPointerEvent& mouseEvent)
{
	// Get current mouse position, relative to the local window:
	auto currentMousePosition = mouseEvent.GetScreenSpacePosition();
	currentMousePosition = USlateBlueprintLibrary::AbsoluteToLocal(geometry, currentMousePosition);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Move, %d, %d"), static_cast<int>(currentMousePosition.X), static_cast<int>(currentMousePosition.Y)));

	// Get the slot where this border is contained:
	auto canvasPanelSlot = Cast<UCanvasPanelSlot>(UWidget::Slot);
	auto slotPosition = canvasPanelSlot->GetPosition();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Canvas Slot Position, %d, %d"), static_cast<int>(slotPosition.X), static_cast<int>(slotPosition.Y)));

	// Resize slot according to anchors:
	{
		auto desiredSize = currentMousePosition - slotPosition;

		auto anchors = canvasPanelSlot->LayoutData.Anchors;
		if (anchors.IsStretchedHorizontal())
			desiredSize.X = anchors.Maximum.X;
		if (anchors.IsStretchedVertical())
			desiredSize.Y = anchors.Maximum.Y;

		canvasPanelSlot->SetSize(desiredSize);
	}
}