// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <atomic>
#include <mutex>
#include <vector>
#include <list>
#include <map>

#include "Blueprint/UserWidget.h"
#include "GrammarListItem.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MICHELANGELO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UGrammarListItem> ListItemWidgetTemplate;

public:
	explicit UMainMenu(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void OnConstruct(UPanelWidget* ownGrammarsContainer, UPanelWidget* sharedGrammarsContainer, UPanelWidget* tutorialsContainer);

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void AddGrammars(const TArray<UGrammarSpecificData*>& grammars);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void CreateNewGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void CreateNewGrammarAsync();

private:
	void HandlePendingGrammars();
	void GetGrammars(EGrammarType type);

private:
	std::atomic<bool> m_busy;
	
private:
	std::map<EGrammarType, UPanelWidget*> m_grammarContainers;

	std::mutex m_pendingGrammarsMutex;
	std::list<MichelangeloAPI::GrammarSpecificData> m_pendingGrammars;
};
