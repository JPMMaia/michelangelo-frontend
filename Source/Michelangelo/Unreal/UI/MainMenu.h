// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <mutex>
#include <deque>
#include <map>

#include "Blueprint/UserWidget.h"
#include "GrammarListItem.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "NonUnreal/Common/Event.h"
#include "NonUnreal/Common/EventsComponent.h"
#include "NonUnreal/Common/TasksComponent.h"
#include "MainMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrammarCreatedEvent, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogOutEvent, bool, Success);

/**
	* 
	*/
UCLASS()
class MICHELANGELO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnGrammarCreatedEvent OnGrammarCreatedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnLogOutEvent OnLogOutEvent;

public:
	explicit UMainMenu(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void OnConstruct(UPanelWidget* ownGrammarsContainer, UPanelWidget* sharedGrammarsContainer, UPanelWidget* tutorialsContainer);

	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void AddGrammars(const TArray<UGrammarSpecificData*>& grammars);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestCreateNewGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestCreateNewGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestLogOut();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestLogOutAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetErrorMessage();

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void HandlePendingGrammars();
	void RequestGrammarsList(EGrammarType type) noexcept;
	void SetErrorMessage(const FString& errorMessage);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UGrammarListItem> ListItemWidgetTemplate;

private:
	std::map<EGrammarType, UPanelWidget*> m_grammarContainers;

	std::mutex m_pendingGrammarsMutex;
	std::deque<MichelangeloAPI::GrammarSpecificData> m_pendingGrammars;

	Common::EventsComponent<UMainMenu> m_eventsComponent;
	Common::TasksComponent m_tasksComponent;

	std::mutex m_errorMessageMutex;
	FString m_errorMessage;
};
