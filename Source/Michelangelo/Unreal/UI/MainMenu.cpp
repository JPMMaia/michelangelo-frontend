// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MainMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "PanelWidget.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "Unreal/Common/UnrealHelpers.h"

#include <vector>
#include <functional>

using namespace Common;
using namespace MichelangeloAPI;

namespace Events
{
	class OnGetGrammarsListEvent : public Common::Event<UMainMenu>
	{
	public:
		explicit OnGetGrammarsListEvent(const std::vector<GrammarSpecificData>& grammars, GrammarType grammarType, bool success) :
			m_grammars(grammars),
			m_grammarType(grammarType),
			m_success(success)
		{
		}

		void Handle(UMainMenu& sender) override
		{
			TArray<UGrammarSpecificData*> unrealGrammars;
			unrealGrammars.Reserve(m_grammars.size());
			for (const auto& grammar : m_grammars)
				unrealGrammars.Add(UGrammarSpecificData::FromNativeData(grammar));

			sender.OnGetGrammarsListEvent.Broadcast(unrealGrammars, Helpers::NativeToUnrealGrammarType(m_grammarType), m_success);
		}

	private:
		std::vector<GrammarSpecificData> m_grammars;
		GrammarType m_grammarType;
		bool m_success;
	};

	class OnGrammarCreatedEvent : public Common::Event<UMainMenu>
	{
	public:
		explicit OnGrammarCreatedEvent(const GrammarSpecificData& grammarData, bool success) :
			m_grammarData(grammarData),
			m_success(success)
		{
		}

		void Handle(UMainMenu& sender) override
		{
			sender.OnGrammarCreatedEvent.Broadcast(UGrammarSpecificData::FromNativeData(m_grammarData), m_success);
		}

	private:
		GrammarSpecificData m_grammarData;
		bool m_success;
	};

	class OnLogOutEvent : public Common::Event<UMainMenu>
	{
	public:
		explicit OnLogOutEvent(bool success) :
			m_success(success)
		{
		}

		void Handle(UMainMenu& sender) override
		{
			sender.OnLogOutEvent.Broadcast(m_success);
		}

	private:
		bool m_success;
	};
}

void UMainMenu::RequestGrammarsList(EGrammarType type)
{
	try
	{
		// Get native web api:
		auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

		// Make web request to get list of grammars of the desired type:
		auto grammars = nativeWebAPI.GetGrammarsList(Helpers::UnrealToNativeGrammarType(type));

		// Sort grammars by date:
		auto sortByDate = [](const GrammarSpecificData& element1, const GrammarSpecificData& element2)
		{
			return element1.LastModified < element2.LastModified;
		};
		std::sort(grammars.begin(), grammars.end(), sortByDate);

		m_eventsComponent.AddEvent(std::make_unique<Events::OnGetGrammarsListEvent>(grammars, Helpers::UnrealToNativeGrammarType(type), true));
	}
	catch (const std::exception& error)
	{
		SetErrorMessage(error.what());
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGetGrammarsListEvent>(std::vector<GrammarSpecificData>(), Helpers::UnrealToNativeGrammarType(type), false));
	}
}
void UMainMenu::RequestGrammarsListAsync()
{
	m_tasksComponent.Add(Task(std::bind(&UMainMenu::RequestGrammarsList, this, EGrammarType::Own)));
	m_tasksComponent.Add(Task(std::bind(&UMainMenu::RequestGrammarsList, this, EGrammarType::Shared)));
	m_tasksComponent.Add(Task(std::bind(&UMainMenu::RequestGrammarsList, this, EGrammarType::Tutorial)));
}

void UMainMenu::RequestCreateNewGrammar()
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	// Make web request to create new grammar:
	try
	{
		auto grammar = nativeWebAPI.CreateNewGrammar();

		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarCreatedEvent>(grammar, true));
	}
	catch (const std::exception& error)
	{
		SetErrorMessage(error.what());
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarCreatedEvent>(GrammarSpecificData(), false));
	}
}
void UMainMenu::RequestCreateNewGrammarAsync()
{
	auto createNewGrammar = std::bind(&UMainMenu::RequestCreateNewGrammar, this);
	m_tasksComponent.Add(Task(createNewGrammar));
}

void UMainMenu::RequestLogOut()
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	try
	{
		nativeWebAPI.LogOut();
	}
	catch (const std::exception& error)
	{
		SetErrorMessage(error.what());
		m_eventsComponent.AddEvent(std::make_unique<Events::OnLogOutEvent>(false));
		return;
	}

	m_eventsComponent.AddEvent(std::make_unique<Events::OnLogOutEvent>(true));
}
void UMainMenu::RequestLogOutAsync()
{
	auto logOut = std::bind(&UMainMenu::RequestLogOut, this);
	m_tasksComponent.Add(Task(logOut));
}

FString UMainMenu::GetErrorMessage()
{
	std::lock_guard<std::mutex> lock(m_errorMessageMutex);
	return m_errorMessage;
}

void UMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Call parent function:
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	// Handle events:
	m_eventsComponent.HandleEvents(*this);

	m_tasksComponent.Update();
}

void UMainMenu::SetErrorMessage(const FString& errorMessage)
{
	std::lock_guard<std::mutex> lock(m_errorMessageMutex);
	m_errorMessage = errorMessage;
}
