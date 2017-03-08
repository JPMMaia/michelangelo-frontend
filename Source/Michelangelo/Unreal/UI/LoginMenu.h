// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DelegateCombinations.h"
#include "NonUnreal/Common/EventsComponent.h"
#include "NonUnreal/Common/TasksComponent.h"
#include "LoginMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAuthenticationEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthenticationErrorEvent, FString, ErrorMessage);

/**
* 
*/
UCLASS()
class MICHELANGELO_API ULoginMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FAuthenticationEvent OnAuthenticated;
	
	UPROPERTY(BlueprintAssignable)
	FAuthenticationErrorEvent OnAuthenticationError;

public:

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestLogIn(const FString& email, const FString& password, bool rememberMe);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestLogInAsync(const FString& email, const FString& password, bool rememberMe);

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	Common::EventsComponent<ULoginMenu> m_eventsComponent;
	Common::TasksComponent m_tasksComponent;
};
