#include "Michelangelo.h"
#include "EdgeSlider.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "ResizableBorder.h"

UEdgeSlider::UEdgeSlider(const FObjectInitializer& ObjectInitializer) :
	UButton(),
	SlidePosition(ESlidePosition::Right)
{
	//OnMouseButtonDownEvent.BindDynamic(this, &UResizableBorder::OnMouseButtonDown);

	UButton::OnHovered.AddDynamic(this, &UEdgeSlider::OnMouseHovered);
	UButton::OnUnhovered.AddDynamic(this, &UEdgeSlider::OnMouseUnhovered);
}

void UEdgeSlider::PostLoad()
{
	Super::PostLoad();

	auto parent = UWidget::GetParent();
	if (parent)
	{
		auto resizableBorder = Cast<UResizableBorder>(parent->GetParent());
		resizableBorder->AddEdgeSlider(this);
	}
}

void UEdgeSlider::OnMouseHovered()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Mouse Hovered")));

	OnEdgeMouseHoveredEvent.ExecuteIfBound(SlidePosition);
}
void UEdgeSlider::OnMouseUnhovered()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Mouse Unhovered")));

	OnEdgeMouseUnhoveredEvent.ExecuteIfBound(SlidePosition);
}