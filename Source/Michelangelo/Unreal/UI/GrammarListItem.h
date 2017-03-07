// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <mutex>

#include "Blueprint/UserWidget.h"
#include "Unreal/Web/GrammarSpecificData.h"
#include "GrammarListItem.generated.h"

/**
 * 
 */
UCLASS()
class MICHELANGELO_API UGrammarListItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void ShareGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void ShareGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void DeleteGrammar();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void DeleteGrammarAsync();

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void SetData(UGrammarSpecificData* data);


public:

	std::mutex m_dataMutex;

	UPROPERTY(BlueprintReadOnly, Category = Michelangelo)
	UGrammarSpecificData* GrammarSpecificData;
};
