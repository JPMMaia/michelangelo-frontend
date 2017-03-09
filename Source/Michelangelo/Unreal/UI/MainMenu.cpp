// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MainMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "PanelWidget.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "Unreal/Common/UnrealHelpers.h"

#include <functional>
#include <future>

using namespace Common;
using namespace MichelangeloAPI;

namespace Events
{
	class OnGrammarCreatedEvent : public Common::Event<UMainMenu>
	{
	public:
		explicit OnGrammarCreatedEvent(bool success) :
			m_success(success)
		{
		}

		void Handle(UMainMenu& sender) override
		{
			sender.OnGrammarCreatedEvent.Broadcast(m_success);
		}

	private:
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

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer),
	ListItemWidgetTemplate(ConstructorHelpers::FClassFinder<UGrammarListItem>(TEXT("/Game/UI/W_GrammarListItem")).Class)
{
}

void UMainMenu::OnConstruct(UPanelWidget* ownGrammarsContainer, UPanelWidget* sharedGrammarsContainer, UPanelWidget* tutorialsContainer)
{
	m_grammarContainers.emplace(EGrammarType::Own, ownGrammarsContainer);
	m_tasksComponent.Add(Task(std::bind(&UMainMenu::RequestGrammarsList, this, EGrammarType::Own)));

	m_grammarContainers.emplace(EGrammarType::Shared, sharedGrammarsContainer);
	m_tasksComponent.Add(Task(std::bind(&UMainMenu::RequestGrammarsList, this, EGrammarType::Shared)));

	m_grammarContainers.emplace(EGrammarType::Tutorial, tutorialsContainer);
	m_tasksComponent.Add(Task(std::bind(&UMainMenu::RequestGrammarsList, this, EGrammarType::Tutorial)));
}
void UMainMenu::NativeDestruct()
{
	for (const auto& pair : m_grammarContainers)
		pair.second->ClearChildren();
	m_grammarContainers.clear();
}

void UMainMenu::AddGrammars(const TArray<UGrammarSpecificData*>& grammars)
{
	auto* gameData = UGameDataSingleton::Get();
	auto* world = gameData->GetSpawner()->GetWorld();

	for (const auto& grammar : grammars)
	{
		// Create list item widget:
		auto* listItemWidget = CreateWidget<UGrammarListItem>(world, ListItemWidgetTemplate.Get());
		listItemWidget->SetGrammarData(grammar);
		
		// Add it to the respective container:
		auto* container = m_grammarContainers.at(grammar->GrammarType);
		container->AddChild(listItemWidget);
	}
}

void UMainMenu::RequestCreateNewGrammar()
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	// Make web request to create new grammar:
	try
	{
		auto grammar = nativeWebAPI.CreateNewGrammar();

		// Lock mutex and add the new grammar to the pending grammars list:
		std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);
		m_pendingGrammars.push_back(grammar);
	}
	catch (const std::exception& error)
	{
		SetErrorMessage(error.what());
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarCreatedEvent>(false));
		return;
	}

	// Notify that grammar was created:
	m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarCreatedEvent>(true));
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

	// Handle pending grammars:
	HandlePendingGrammars();
}

void UMainMenu::HandlePendingGrammars()
{
	TArray<UGrammarSpecificData*> unrealGrammars;
	{
		// Lock pending grammars mutex:
		std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);

		// Nothing to do if there aren't any pending grammars:
		if (m_pendingGrammars.empty())
			return;

		// Convert native to unreal grammars:
		while(!m_pendingGrammars.empty())
		{
			unrealGrammars.Add(UGrammarSpecificData::FromNativeData(m_pendingGrammars.front()));
			m_pendingGrammars.pop_front();
		}
	}
	
	// Add the grammars to the respective containers:
	AddGrammars(unrealGrammars);
}
void UMainMenu::RequestGrammarsList(EGrammarType type) noexcept
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

		// Lock mutex and add the fetched grammars to the pending grammars list:
		std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);
		m_pendingGrammars.insert(std::end(m_pendingGrammars), std::begin(grammars), std::end(grammars));
	}
	catch (const std::exception& error)
	{
		SetErrorMessage(error.what());
	}
}
void UMainMenu::SetErrorMessage(const FString& errorMessage)
{
	std::lock_guard<std::mutex> lock(m_errorMessageMutex);
	m_errorMessage = errorMessage;
}