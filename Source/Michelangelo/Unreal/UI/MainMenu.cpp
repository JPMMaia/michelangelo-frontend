// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MainMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "Unreal/Web/UWebAPI.h"
#include "PanelWidget.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "NonUnreal/MichelangeloAPI/URLConstants.h"
#include "Unreal/Common/UnrealHelpers.h"

#include <functional>
#include <future>

using namespace Common;
using namespace MichelangeloAPI;

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer),
	ListItemWidgetTemplate(ConstructorHelpers::FClassFinder<UGrammarListItem>(TEXT("/Game/UI/W_GrammarListItem")).Class),
	m_busy(false)
{
}

void UMainMenu::OnConstruct(UPanelWidget* ownGrammarsContainer, UPanelWidget* sharedGrammarsContainer, UPanelWidget* tutorialsContainer)
{
	m_grammarContainers.emplace(EGrammarType::Own, ownGrammarsContainer);
	m_grammarContainers.emplace(EGrammarType::Shared, sharedGrammarsContainer);
	m_grammarContainers.emplace(EGrammarType::Tutorial, tutorialsContainer);

	auto getGrammarsAsync = [this]()
	{
		GetGrammars(EGrammarType::Own);
		GetGrammars(EGrammarType::Shared);
		GetGrammars(EGrammarType::Tutorial);
	};
	std::thread(getGrammarsAsync).detach();
}

void UMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Call parent function:
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

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
		auto* container = m_grammarContainers.at(grammar->GetGrammarType());
		container->AddChild(listItemWidget);
	}
}

void UMainMenu::CreateNewGrammar()
{
	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI()->GetNativeWebAPI();

	// Make web request to create new grammar:
	auto grammar = nativeWebAPI.CreateNewGrammar();

	// Lock mutex and add the new grammar to the pending grammars list:
	std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);
	m_pendingGrammars.push_back(grammar);
}
void UMainMenu::CreateNewGrammarAsync()
{
	auto createNewGrammar = std::bind(&UMainMenu::CreateNewGrammar, this);
	std::thread(createNewGrammar).detach();
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
		for (const auto& nativeGrammar : m_pendingGrammars)
			unrealGrammars.Add(UGrammarSpecificData::FromApiData(nativeGrammar));

		// Clear pending grammars list:
		m_pendingGrammars.clear();
	}
	
	// Add the grammars to the respective containers:
	AddGrammars(unrealGrammars);
}
void UMainMenu::GetGrammars(EGrammarType type)
{
	// Get web apis:
	auto* webAPI = UGameDataSingleton::Get()->GetWebAPI();
	auto& nativeWebAPI = webAPI->GetNativeWebAPI();

	// Select url according to the type of grammar:
	auto url = Helpers::FStringToString(webAPI->SelectGetGrammarsURL(type));

	// Make web request to get list of grammars of the desired type:
	auto grammars = nativeWebAPI.GetGrammars(url);

	// Lock mutex and add the fetched grammars to the pending grammars list:
	std::lock_guard<std::mutex> lock(m_pendingGrammarsMutex);
	m_pendingGrammars.insert(std::end(m_pendingGrammars), std::begin(grammars), std::end(grammars));
}