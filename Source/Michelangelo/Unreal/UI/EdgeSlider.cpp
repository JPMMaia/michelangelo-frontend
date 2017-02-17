#include "Michelangelo.h"
#include "EdgeSlider.h"
#include <Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "ResizableBorder.h"
#include <thread>

UEdgeSlider::UEdgeSlider(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer),
	SlidePosition(ESlidePosition::Right)
{
	/*UButton::OnHovered.AddDynamic(this, &UEdgeSlider::OnMouseHover);
	UButton::OnUnhovered.AddDynamic(this, &UEdgeSlider::OnMouseUnhover);
	UButton::OnPressed.AddDynamic(this, &UEdgeSlider::OnMousePress);
	UButton::OnReleased.AddDynamic(this, &UEdgeSlider::OnMouseRelease);*/
}

void UEdgeSlider::PostLoad()
{
	Super::PostLoad();

	/*auto parent = UWidget::GetParent();
	if (parent)
	{
		auto resizableBorder = Cast<UResizableBorder>(parent->GetParent());
		resizableBorder->AddEdgeSlider(this);
	}*/
}

void UEdgeSlider::OnMouseHover()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Hover")));
}
void UEdgeSlider::OnMouseUnhover()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Unhover")));
}
void UEdgeSlider::OnMousePress()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Press")));

	// Detect if a user starts dragging in this widget later, if the left mouse button is clicked:
	/*auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton);
	UWidgetBlueprintLibrary::CaptureMouse(detectDragReply, this);*/

	auto lambda = [this]()
	{
		using namespace std::chrono_literals;

		GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Lambda")));
		/*while(this->IsPressed())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Lambda2")));



			std::this_thread::sleep_for(30ms);
		}*/
	};
	std::thread(lambda).detach();
}
void UEdgeSlider::OnMouseRelease()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, FString::Printf(TEXT("Release")));

	// Detect if a user starts dragging in this widget later, if there is mouse movement:
	/*auto detectDragReply = UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::MouseX);
	UWidgetBlueprintLibrary::ReleaseMouseCapture(detectDragReply);*/
}
