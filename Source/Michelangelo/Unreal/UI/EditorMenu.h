// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <deque>
#include <mutex>
#include <string>

#include "Blueprint/UserWidget.h"
#include "EditorMenu.generated.h"

class UGrammarSpecificData;

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
	
private:
	enum class InternalEventType
	{
		GrammarEvaluated,
		GrammarError,
		GrammarWarning
	};

	struct InternalEvent
	{
	public:
		InternalEventType Type;
		std::string Message;

	public:
		static InternalEvent OnGrammarEvaluatedEvent()
		{
			return { InternalEventType::GrammarEvaluated, "" };
		}
		static InternalEvent OnGrammarErrorEvent(const std::string& message)
		{
			return { InternalEventType::GrammarError, message };
		}
		static InternalEvent OnGrammarWarningEvent(const std::string& message)
		{
			return { InternalEventType::GrammarWarning, message };
		}
	};

public:
	UPROPERTY(BlueprintAssignable)
	FOnGrammarEvaluatedEvent OnGrammarEvaluatedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarErrorEvent OnGrammarErrorEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarWarningEvent OnGrammarWarningEvent;

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void EvaluateGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void EvaluateGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void SetGrammarData(UGrammarSpecificData* value);

private:
	void AddEventToQueue(InternalEvent&& internalEvent);

public:
	UPROPERTY(BlueprintReadOnly, Category = Michelangelo)
	UGrammarSpecificData* GrammarData;

private:
	std::mutex m_eventsQueueMutex;
	std::deque<InternalEvent> m_eventsQueue;
};
