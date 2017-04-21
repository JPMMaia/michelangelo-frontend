// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <deque>
#include <mutex>
#include <string>

#include "Blueprint/UserWidget.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"
#include "NonUnreal/Common/EventsComponent.h"
#include "NonUnreal/Common/TasksComponent.h"
#include "EditorMenu.generated.h"

class UGrammarSpecificData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEditorReadyEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrammarEvaluatedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrammarErrorEvent, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrammarWarningEvent, FString, WarningMessage);

/**
 *
 */
UCLASS()
class MICHELANGELO_API UEditorMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnEditorReadyEvent OnEditorReadyEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarEvaluatedEvent OnGrammarEvaluatedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarErrorEvent OnGrammarErrorEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarWarningEvent OnGrammarWarningEvent;

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestEvaluateGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestEvaluateGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestGrammarData();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestGrammarDataAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void SetGrammarData(UGrammarSpecificData* value);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetName() const;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetType() const;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetCode() const;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void SetCode(const FString& value);

	void HandleSceneGeometry(const MichelangeloAPI::SceneGeometry& sceneGeometry);

private:
	static void FormatText(std::string& message);

private:
	std::mutex m_grammarDataMutex;

	UPROPERTY()
	UGrammarSpecificData* m_grammarData;

	Common::EventsComponent<UEditorMenu> m_eventsComponent;
	Common::TasksComponent m_tasksComponent;
};
