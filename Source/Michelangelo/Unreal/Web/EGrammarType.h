#pragma once

#include "EGrammarType.generated.h"

UENUM(BlueprintType)
enum class EGrammarType : uint8
{
	Unknown     UMETA(DisplayName = "Unknown"),
	Own 		UMETA(DisplayName = "Own"),
	Shared 		UMETA(DisplayName = "Shared"),
	Tutorial	UMETA(DisplayName = "Tutorial")
};