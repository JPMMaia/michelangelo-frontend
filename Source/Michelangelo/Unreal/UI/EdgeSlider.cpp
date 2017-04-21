#include "Michelangelo.h"
#include "EdgeSlider.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "WidgetLayoutLibrary.h"

UEdgeSlider::UEdgeSlider(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer),
	m_focus(false)
{
}

void UEdgeSlider::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	// Get player controller:
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController)
	{
		// Bind to drag release events:
		playerController->InputComponent->BindAction("Drag", IE_Released, this, &UEdgeSlider::OnDragReleased);
	}
}
void UEdgeSlider::OnConstruct(UPanelWidget* widgetToResize)
{
	m_widgetToResize = widgetToResize;
}

void UEdgeSlider::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::Tick(MyGeometry, InDeltaTime);

	if (!m_focus || !m_widgetToResize)
		return;

	// Get parent:
	auto parent = GetParent();
	if (!parent)
		return;

	// Get player controller:
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Get current mouse position:
	FVector2D currentMousePosition;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(playerController, currentMousePosition.X, currentMousePosition.Y);

	// Calculate the difference between the last and the current mouse positions:
	auto offset = (currentMousePosition - m_lastMousePosition);

	// Set last mouse position for next call:
	m_lastMousePosition = currentMousePosition;

	// Resize slot:
	auto* slot = Cast<UCanvasPanelSlot>(m_widgetToResize->Slot);
	auto anchors = slot->LayoutData.Anchors;
	auto desiredSize = slot->GetSize();
	if (!anchors.IsStretchedHorizontal())
		desiredSize.X += offset.X;
	if (!anchors.IsStretchedVertical())
		desiredSize.Y += offset.Y;
	slot->SetSize(desiredSize);
}

FReply UEdgeSlider::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Set focus:
	m_focus = true;

	// Get current mouse position:
	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(player, m_lastMousePosition.X, m_lastMousePosition.Y);

	return FReply::Handled();
}

void UEdgeSlider::OnDragReleased()
{
	// Release focus:
	m_focus = false;
}
