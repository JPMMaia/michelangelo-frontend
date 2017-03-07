// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <atomic>
#include <mutex>
#include <vector>
#include <deque>
#include <map>

#include "Blueprint/UserWidget.h"
#include "GrammarListItem.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MICHELANGELO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	explicit UMainMenu(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void OnConstruct(UPanelWidget* ownGrammarsContainer, UPanelWidget* sharedGrammarsContainer, UPanelWidget* tutorialsContainer);

	void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void AddGrammars(const TArray<UGrammarSpecificData*>& grammars);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void CreateNewGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void CreateNewGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void LogOut();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void LogOutAsync();

private:
	void HandlePendingGrammars();
	void GetGrammars(EGrammarType type);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UGrammarListItem> ListItemWidgetTemplate;

private:
	std::map<EGrammarType, UPanelWidget*> m_grammarContainers;

	std::mutex m_pendingGrammarsMutex;
	std::deque<MichelangeloAPI::GrammarSpecificData> m_pendingGrammars;
};
