#include "Michelangelo.h"
#include "ResizableBorder.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>

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

	// Execute On Border Dragging event:
	OnBorderDraggingEvent.ExecuteIfBound(mouseEvent);

	return UWidgetBlueprintLibrary::CaptureMouse(detectDragReply, this);
}
