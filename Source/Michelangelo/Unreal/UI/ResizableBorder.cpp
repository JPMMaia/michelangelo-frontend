#include "Michelangelo.h"
#include "ResizableBorder.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/UMG/Public/Components/CanvasPanelSlot.h>
#include "EdgeSlider.h"

UResizableBorder::UResizableBorder(const FObjectInitializer& ObjectInitializer) :
	UBorder(),
	m_mouseButtonDown(false)
{
	OnMouseButtonDownEvent.BindDynamic(this, &UResizableBorder::OnMouseButtonDown);
	OnMouseButtonUpEvent.BindDynamic(this, &UResizableBorder::OnMouseButtonUp);
	OnMouseMoveEvent.BindDynamic(this, &UResizableBorder::OnMouseMove);

	//m_rightEdge = ObjectInitializer.CreateDefaultSubobject<UButton>(this, TEXT("RightEdge"));
	//UPanelWidget::AddChild(m_rightEdge);
}

void UResizableBorder::AddEdgeSlider(UEdgeSlider* edgeSlider)
{
	m_edgeSliders.Add(edgeSlider);

	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Edge Added")));
}

FEventReply UResizableBorder::OnMouseButtonDown(FGeometry geometry, const FPointerEvent& mouseEvent)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Down")));

	m_mouseButtonDown = true;

	// Detect if a user starts dragging in this widget later, if the left mouse button is clicked:
	auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton);

	return UWidgetBlueprintLibrary::CaptureMouse(detectDragReply, this);*/
	return FEventReply(true);
}

FEventReply UResizableBorder::OnMouseButtonUp(FGeometry geometry, const FPointerEvent& mouseEvent)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Up")));

	m_mouseButtonDown = false;

	// Detect if a user starts dragging in this widget later, if there is mouse movement:
	auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::MouseX);

	return UWidgetBlueprintLibrary::ReleaseMouseCapture(detectDragReply);*/

	return FEventReply(true);
}

void UResizableBorder::OnEdgeMouseHovered(ESlidePosition slidePosition)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Edge Hovered Received: %d"), static_cast<uint8>(slidePosition)));

	m_hoveringEdgeSlide = true;
	m_edgeSlidePosition = slidePosition;*/
}
void UResizableBorder::OnEdgeMouseUnhovered(ESlidePosition slidePosition)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Edge Unhovered Received: %d"), static_cast<uint8>(slidePosition)));

	m_hoveringEdgeSlide = false;*/
}

FEventReply UResizableBorder::OnMouseMove(FGeometry geometry, const FPointerEvent& mouseEvent)
{
	/*if (!m_mouseButtonDown)
		return FEventReply(true);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Mouse Move")));

	// Detect if a user starts dragging in this widget later, if the left mouse button is clicked:
	auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton);

	if(m_hoveringEdgeSlide && m_mouseButtonDown)
	{
		// Resize widget according to the mouse position:
		ResizeAccordingToMousePosition(geometry, mouseEvent);
	}

	return UWidgetBlueprintLibrary::CaptureMouse(detectDragReply, this);*/
	return FEventReply(true);
}

void UResizableBorder::ResizeAccordingToMousePosition(const FGeometry& geometry, const FPointerEvent& mouseEvent)
{
	// Get the position of the slot where this border is contained:
	auto parent = Cast<UCanvasPanelSlot>(UWidget::Slot);
	auto parentPosition = parent->GetPosition();
	
	// Resize slot according to anchors:
	auto geometryPosition = geometry.Position;
	auto geometrySize = geometry.Size;
	auto mousePosition = geometry.AbsoluteToLocal(mouseEvent.GetScreenSpacePosition());
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Mouse Position, %d, %d"), static_cast<int>(mousePosition.X), static_cast<int>(mousePosition.Y)));

	auto anchors = parent->LayoutData.Anchors;
	auto desiredSize = parent->GetSize();
	if (!anchors.IsStretchedHorizontal())
	{
		desiredSize.X = geometryPosition.X + geometrySize.X + mousePosition.X;
	}

	if (!anchors.IsStretchedVertical())
	{
		desiredSize.Y = geometryPosition.Y + geometrySize.Y + mousePosition.Y;
	}

	parent->SetSize(desiredSize);
}
FVector2D UResizableBorder::GetMouseLocalPosition(const FGeometry& geometry, const FPointerEvent& mouseEvent) const
{
	auto mouseAbsolutePosition = mouseEvent.GetScreenSpacePosition();
	return USlateBlueprintLibrary::AbsoluteToLocal(geometry, mouseAbsolutePosition);
}
