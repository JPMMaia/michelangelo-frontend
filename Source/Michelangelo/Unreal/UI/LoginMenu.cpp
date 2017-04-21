// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "LoginMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "NonUnreal/MichelangeloAPI/Exceptions/AuthenticationError.h"

using namespace Common;

namespace Events
{
	class OnAuthenticatedEvent : public Common::Event<ULoginMenu>
	{
	public:
		void Handle(ULoginMenu& sender) override
		{
			sender.OnAuthenticated.Broadcast();
		}
	};

	class OnErrorEvent : public Common::Event<ULoginMenu>
	{
	public:
		explicit OnErrorEvent(const std::string& message) :
			m_message(message)
		{
		}

		void Handle(ULoginMenu& sender) override
		{
			sender.OnAuthenticationError.Broadcast(Helpers::StringToFString(m_message));
		}

	private:
		std::string m_message;
	};
}

void ULoginMenu::RequestLogIn(const FString& email, const FString& password, bool rememberMe)
{
	if (email.IsEmpty())
	{
		m_eventsComponent.AddEvent(std::make_unique<Events::OnErrorEvent>("Email field must not be empty!"));
		return;
	}

	if (password.IsEmpty())
	{
		m_eventsComponent.AddEvent(std::make_unique<Events::OnErrorEvent>("Password field must not be empty!"));
		return;
	}

	auto* gameData = UGameDataSingleton::Get();
	if (rememberMe)
	{
		// Save email for future use:
		auto gameDataSingleton = UGameDataSingleton::Get();
		gameDataSingleton->SetSavedEmail(email);
	}

	auto& nativeWebAPI = gameData->GetWebAPI();

	try
	{
		nativeWebAPI.Authenticate(Helpers::FStringToString(email), Helpers::FStringToString(password), rememberMe);
	}
	catch (const MichelangeloAPI::AuthenticationError& error)
	{
		m_eventsComponent.AddEvent(std::make_unique<Events::OnErrorEvent>(error.what()));
		return;
	}
	catch (const std::exception&)
	{
		m_eventsComponent.AddEvent(std::make_unique<Events::OnErrorEvent>("Unexpected error!"));
		return;
	}

	// Broadcast on authenticated event:
	m_eventsComponent.AddEvent(std::make_unique<Events::OnAuthenticatedEvent>());
}
void ULoginMenu::RequestLogInAsync(const FString& email, const FString& password, bool rememberMe)
{
	auto async = std::bind(&ULoginMenu::RequestLogIn, this, email, password, rememberMe);
	m_tasksComponent.Add(Task(async));
}

void ULoginMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	m_eventsComponent.HandleEvents(*this);
	
	m_tasksComponent.Update();
}
