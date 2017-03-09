// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <mutex>

#include "Blueprint/UserWidget.h"
#include "GrammarListItem.h"
#include "Array.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "NonUnreal/Common/Event.h"
#include "NonUnreal/Common/EventsComponent.h"
#include "NonUnreal/Common/TasksComponent.h"
#include "MainMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGetGrammarsListEvent, const TArray<UGrammarSpecificData*>&, GrammarsList, EGrammarType, Type, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGrammarCreatedEvent, UGrammarSpecificData*, GrammarData, bool, Success);
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
	FOnGetGrammarsListEvent OnGetGrammarsListEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarCreatedEvent OnGrammarCreatedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnLogOutEvent OnLogOutEvent;

public:

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestGrammarsList(EGrammarType type);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestGrammarsListAsync();

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
	void SetErrorMessage(const FString& errorMessage);

private:
	Common::EventsComponent<UMainMenu> m_eventsComponent;
	Common::TasksComponent m_tasksComponent;

	std::mutex m_errorMessageMutex;
	FString m_errorMessage;
};
