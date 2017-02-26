#include "Michelangelo.h"
#include "ResizableBorder.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/UMG/Public/Components/CanvasPanelSlot.h>
#include "EdgeSlider.h"
#include "Unreal/UGameDataSingletonLibrary.h"

void UResizableBorder::ResizeAccordingToMousePosition(const FGeometry& geometry, const FPointerEvent& mouseEvent)
{
	auto parent = Cast<UCanvasPanel>(GetParent());
	auto slot = Cast<UCanvasPanelSlot>(UWidget::Slot);
	FGeometry slotGeometry;
	parent->GetGeometryForSlot(slot, slotGeometry);
	
	FVector2D mousePosition;
	GEngine->GameViewport->GetMousePosition(mousePosition);
	mousePosition = slotGeometry.AbsoluteToLocal(mousePosition);
	//USlateBlueprintLibrary::ScreenToViewport(this, mouseEvent.GetScreenSpacePosition(), mousePosition);
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Mouse Position, %d, %d"), static_cast<int>(mousePosition.X), static_cast<int>(mousePosition.Y)));

	
	
	auto anchors = slot->LayoutData.Anchors;
	auto desiredSize = slot->GetSize();
	


	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Current Size, %d, %d"), static_cast<int>(desiredSize.X), static_cast<int>(desiredSize.Y)));
	
	auto slotPosition = slot->GetPosition();
	auto newSize = mousePosition - slot->GetPosition() + geometry.GetLocalSize();
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("New Size, %d, %d"), static_cast<int>(newSize.X), static_cast<int>(newSize.Y)));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Slot Position, %d, %d"), static_cast<int>(slotPosition.X), static_cast<int>(slotPosition.Y)));


	if (!anchors.IsStretchedHorizontal())
	{
		desiredSize.X = newSize.X;
	}

	if (!anchors.IsStretchedVertical())
	{
		desiredSize.Y = newSize.Y;
	}
	slot->SetSize(desiredSize);

	/*// Get the position of the slot where this border is contained:
	auto parent = Cast<UCanvasPanelSlot>(UWidget::Slot);
	auto x = Cast<UCanvasPanel>(GetParent());

	x->GetGeometryForSlot(parent, );
	auto parentPosition = parent->GetPosition();
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Canvas Panel Position, %d, %d"), static_cast<int>(parentPosition.X), static_cast<int>(parentPosition.Y)));
	
	// Resize slot according to anchors:
	auto geometryPosition = geometry.Position;
	auto geometrySize = geometry.Size;
	auto mousePosition = geometry.AbsoluteToLocal(mouseEvent.GetScreenSpacePosition());
	//GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Mouse Position, %d, %d"), static_cast<int>(mousePosition.X), static_cast<int>(mousePosition.Y)));

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

	parent->SetSize(desiredSize);*/
}
FVector2D UResizableBorder::GetMouseLocalPosition(const FGeometry& geometry, const FPointerEvent& mouseEvent) const
{
	auto mouseAbsolutePosition = mouseEvent.GetScreenSpacePosition();
	return USlateBlueprintLibrary::AbsoluteToLocal(geometry, mouseAbsolutePosition);
}
