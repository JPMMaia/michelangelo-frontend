// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Unreal/Web/GrammarSpecificData.h"
#include "Unreal/Web/EGrammarType.h"
#include "GrammarListItem.generated.h"

/**
 * 
 */
UCLASS()
class MICHELANGELO_API UGrammarListItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Michelangelo)
	UGrammarSpecificData* GrammarSpecificData;

	UFUNCTION(BlueprintCallable, Category = "Michelangelo")
	void SetData(UGrammarSpecificData* data);
};
