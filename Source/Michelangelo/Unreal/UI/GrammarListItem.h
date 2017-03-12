#pragma once

#include <mutex>

#include "Blueprint/UserWidget.h"
#include "Unreal/Web/GrammarSpecificData.h"
#include "NonUnreal/Common/EventsComponent.h"
#include "NonUnreal/Common/TasksComponent.h"
#include "GrammarListItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrammarDataChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrammarSharedEvent, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrammarDeletedEvent, bool, Success);

/**
	* 
	*/
UCLASS()
class MICHELANGELO_API UGrammarListItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnGrammarDataChanged OnGrammarDataChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarSharedEvent OnGrammarSharedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGrammarDeletedEvent OnGrammarDeletedEvent;

public:
	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestShareGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestShareGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestDeleteGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void RequestDeleteGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	UGrammarSpecificData* GetGrammarData();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	bool IsGrammarOwner();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	bool IsGrammarShared();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetGrammarName();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void SetGrammarData(UGrammarSpecificData* data);

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetGrammarID();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetGrammarType();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FDateTime GetGrammarLastModified();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetGrammarLastModifiedFormattedString();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	FString GetErrorMessage();

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void SetErrorMessage(const FString& errorMessage);

private:

	std::mutex m_grammarDataMutex;

	UPROPERTY()
	UGrammarSpecificData* m_grammarData;

	Common::EventsComponent<UGrammarListItem> m_eventsComponent;
	Common::TasksComponent m_tasksComponent;

	std::mutex m_errorMesssageMutex;
	FString m_errorMessage;
};
