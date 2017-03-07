// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <deque>
#include <mutex>
#include <string>

#include "Blueprint/UserWidget.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"
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

private:
	enum class InternalEventType
	{
		EditorReady,
		GrammarEvaluated,
		GrammarError,
		GrammarWarning
	};

	struct InternalEvent
	{
	public:
		InternalEventType Type;
		std::string Message;
		MichelangeloAPI::GrammarSpecificData GrammarData;
		MichelangeloAPI::SceneGeometry SceneGeometry;

	public:
		InternalEvent(InternalEventType type, const std::string& message) :
			Type(type),
			Message(message)
		{
		}
		InternalEvent(InternalEventType type, const MichelangeloAPI::GrammarSpecificData& grammarData) :
			Type(type),
			GrammarData(grammarData)
		{
		}
		InternalEvent(InternalEventType type, const MichelangeloAPI::SceneGeometry& sceneGeometry) :
			Type(type),
			SceneGeometry(sceneGeometry)
		{
		}

		static InternalEvent OnEditorReady(const MichelangeloAPI::GrammarSpecificData& grammarData)
		{
			return { InternalEventType::EditorReady, grammarData };
		}
		static InternalEvent OnGrammarEvaluatedEvent(const MichelangeloAPI::SceneGeometry& sceneGeometry)
		{
			return { InternalEventType::GrammarEvaluated, sceneGeometry };
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
	void EvaluateGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void EvaluateGrammarAsync();

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

private:
	void AddEventToQueue(InternalEvent&& internalEvent);
	static void HandleSceneGeometry(const MichelangeloAPI::SceneGeometry& sceneGeometry);

private:
	std::mutex m_grammarDataMutex;

	UPROPERTY()
		UGrammarSpecificData* m_grammarData;

	std::mutex m_eventsQueueMutex;
	std::deque<InternalEvent> m_eventsQueue;
};
