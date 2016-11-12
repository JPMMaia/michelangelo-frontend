#pragma once

#include "EGrammarType.generated.h"

UENUM(BlueprintType)
enum class EGrammarType : uint8
{
	Own 		UMETA(DisplayName = "Own"),
	Shared 		UMETA(DisplayName = "Shared"),
	Tutorial	UMETA(DisplayName = "Tutorial")
};