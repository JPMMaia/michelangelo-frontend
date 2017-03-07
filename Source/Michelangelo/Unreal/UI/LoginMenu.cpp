// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "LoginMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "Unreal/Web/UWebAPI.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "NonUnreal/MichelangeloAPI/AuthenticationError.h"

using namespace Common;

void ULoginMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	// Lock events queue mutex:
	std::lock_guard<std::mutex> lock(m_eventsQueueMutex);

	while(!m_eventsQueue.empty())
	{
		const auto& event = m_eventsQueue.front();

		if (event.Type == InternalEventType::Authenticated)
			OnAuthenticated.Broadcast();
		else if (event.Type == InternalEventType::Error)
			OnAuthenticationError.Broadcast(Helpers::StringToFString(event.Message));

		m_eventsQueue.pop();
	}
}

void ULoginMenu::Authenticate(const FString& email, const FString& password, bool rememberMe)
{
	if (email.IsEmpty())
	{
		AddEventToQueue(InternalEvent::OnErrorEvent("Email field must not be empty!"));
		return;
	}

	if (password.IsEmpty())
	{
		AddEventToQueue(InternalEvent::OnErrorEvent("Password field must not be empty!"));
		return;
	}

	auto* gameData = UGameDataSingleton::Get();
	if (rememberMe)
	{
		// Save email for future use:
		auto gameDataSingleton = UGameDataSingleton::Get();
		gameDataSingleton->SetSavedEmail(email);
	}

	auto& nativeWebAPI = gameData->GetWebAPI()->GetNativeWebAPI();

	try
	{
		nativeWebAPI.Authenticate(Helpers::FStringToString(email), Helpers::FStringToString(password), rememberMe);
	}
	catch (const MichelangeloAPI::AuthenticationError& error)
	{
		AddEventToQueue(InternalEvent::OnErrorEvent(error.what()));
		return;
	}
	catch (const std::exception&)
	{
		AddEventToQueue(InternalEvent::OnErrorEvent("Unexpected error!"));
		return;
	}

	// Broadcast on authenticated event:
	AddEventToQueue(InternalEvent::OnAuthenticatedEvent());
}
void ULoginMenu::AuthenticateAsync(const FString& email, const FString& password, bool rememberMe)
{
	auto async = std::bind(&ULoginMenu::Authenticate, this, email, password, rememberMe);

	std::thread(async).detach();
}

void ULoginMenu::AddEventToQueue(InternalEvent&& event)
{
	// Lock events queue mutex:
	std::lock_guard<std::mutex> lock(m_eventsQueueMutex);

	// Add event to queue:
	m_eventsQueue.push(event);
}

