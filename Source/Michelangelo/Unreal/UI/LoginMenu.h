// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>
#include <mutex>

#include "Blueprint/UserWidget.h"
#include "DelegateCombinations.h"
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
	
private:
	enum class InternalEventType
	{
		Authenticated,
		Error
	};

	struct InternalEvent
	{
	public:
		InternalEventType Type;
		std::string Message;

	public:
		static InternalEvent OnAuthenticatedEvent()
		{
			return { InternalEventType::Authenticated, "" };
		}
		static InternalEvent OnErrorEvent(const std::string& message)
		{
			return { InternalEventType::Error, message };
		}
	};

public:
	UPROPERTY(BlueprintAssignable)
	FAuthenticationEvent OnAuthenticated;
	
	UPROPERTY(BlueprintAssignable)
	FAuthenticationErrorEvent OnAuthenticationError;

public:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void Authenticate(const FString& email, const FString& password, bool rememberMe);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void AuthenticateAsync(const FString& email, const FString& password, bool rememberMe);

private:
	void AddEventToQueue(InternalEvent&& event);

private:
	std::mutex m_eventsQueueMutex;
	std::queue<InternalEvent> m_eventsQueue;
};
