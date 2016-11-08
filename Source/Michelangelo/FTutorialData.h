// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FTutorialData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MICHELANGELO_API FTutorialData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial Data")
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial Data")
	FString Name;

	FTutorialData();

	const FString& GetName() const;
};
