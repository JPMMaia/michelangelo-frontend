// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MainMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "PanelWidget.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "Unreal/Common/UnrealHelpers.h"

#include <functional>
#include <future>
#include "NonUnreal/MichelangeloAPI/URLConstants.h"

using namespace Common;
using namespace MichelangeloAPI;

class OnGrammarCreatedInternalEvent : public Common::Event<UMainMenu>
{
public:
	void Handle(UMainMenu& sender) override
	{
		sender.OnGrammarCreatedEvent.Broadcast();
	}
};

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer),
	ListItemWidgetTemplate(ConstructorHelpers::FClassFinder<UGrammarListItem>(TEXT("/Game/UI/W_GrammarListItem")).Class)
{
}

void UMainMenu::OnConstruct(UPanelWidget* ownGrammarsContainer, UPanelWidget* sharedGrammarsContainer, UPanelWidget* tutorialsContainer)
{
	m_grammarContainers.emplace(EGrammarType::Own, ownGrammarsContainer);
	std::thread(std::bind(&UMainMenu::GetGrammars, this, EGrammarType::Own)).detach();

	m_grammarContainers.emplace(EGrammarType::Shared, sharedGrammarsContainer);
	std::thread(std::bind(&UMainMenu::GetGrammars, this, EGrammarType::Shared)).detach();

	m_grammarContainers.emplace(EGrammarType::Tutorial, tutorialsContainer);
	std::thread(std::bind(&UMainMenu::GetGrammars, this, EGrammarType::Tutorial)).detach();
}
void UMainMenu::NativeDestruct()
{
	for (const auto& pair : m_grammarContainers)
		pair.second->ClearChildren();
	m_grammarContainers.clear();
}

void UMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Call parent function:
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	// Handle events:
	m_eventsComponent.HandleEvents(*this);

	// Handle pending grammars:
	HandlePendingGrammars();
}

void UMainMenu::AddGrammars(const TArray<UGrammarSpecificData*>& grammars)
{
	auto* gameData = UGameDataSingleton::Get();
	auto* world = gameData->GetSpawner()->GetWorld();

	for (const auto& grammar : grammars)
	{
		// Create list item widget:
		auto* listItemWidget = CreateWidget<UGrammarListItem>(world, ListItemWidgetTemplate.Get());
		listItemWidget->SetData(grammar);
		
		// Add it to the respective container:
		auto* container = m_grammarContainers.at(grammar->GrammarType);
		container->AddChild(listItemWidget);
	}
}

void UMainMenu::CreateNewGrammar()
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	// Make web request to create new grammar:
	auto grammar = nativeWebAPI.CreateNewGrammar();

	// Lock mutex and add the new grammar to the pending grammars list:
	std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);
	m_pendingGrammars.push_back(grammar);

	// Notify that grammar was created:
	m_eventsComponent.AddEvent(std::make_unique<OnGrammarCreatedInternalEvent>());
}
void UMainMenu::CreateNewGrammarAsync()
{
	auto createNewGrammar = std::bind(&UMainMenu::CreateNewGrammar, this);
	std::thread(createNewGrammar).detach();
}

void UMainMenu::LogOut()
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	try
	{
		nativeWebAPI.LogOut();
	}
	catch (const std::exception&)
	{
	}
}
void UMainMenu::LogOutAsync()
{
	auto logOut = std::bind(&UMainMenu::LogOut, this);
	std::thread(logOut).detach();
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
void UMainMenu::GetGrammars(EGrammarType type)
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	// Make web request to get list of grammars of the desired type:
	auto grammars = nativeWebAPI.GetGrammarsList(Helpers::UnrealToNativeGrammarType(type));

	// Lock mutex and add the fetched grammars to the pending grammars list:
	std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);
	m_pendingGrammars.insert(std::end(m_pendingGrammars), std::begin(grammars), std::end(grammars));
}