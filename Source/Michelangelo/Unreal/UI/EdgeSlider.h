#pragma once

#include <Runtime/UMG/Public/UMG.h>
#include <Runtime/UMG/Public/UMGStyle.h>
#include <Runtime/UMG/Public/Components/Widget.h>
#include <Runtime/UMG/Public/Components/Button.h>
#include <Runtime/Core/Public/Delegates/Delegate.h>
#include <Runtime/Core/Public/Delegates/DelegateCombinations.h>
#include "SlidePosition.h"
#include "EdgeSlider.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEdgeMouseHoveredEvent, ESlidePosition, SlidePosition);

UCLASS()
class UEdgeSlider : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour", meta = (DisplayName = "Slide Position"))
	ESlidePosition SlidePosition;

public:
	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	FOnEdgeMouseHoveredEvent OnEdgeMouseHoveredEvent;

	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
	FOnEdgeMouseHoveredEvent OnEdgeMouseUnhoveredEvent;

public:
	explicit UEdgeSlider(const FObjectInitializer& initializer);

public:
	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface

public:

	UFUNCTION()
	void OnMouseHover();

	UFUNCTION()
	void OnMouseUnhover();

	UFUNCTION()
	void OnMousePress();

	UFUNCTION()
	void OnMouseRelease();
};