#pragma once

#include "Unreal/UGameDataSingleton.h"
#include "UGameDataSingletonLibrary.generated.h"

//note about UBlueprintFunctionLibrary
// This class is a base class for any function libraries exposed to blueprints.
// Methods in subclasses are expected to be static, and no methods should be added to the base class.

UCLASS()
class MICHELANGELO_API UGameDataSingletonLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UGameDataSingletonLibrary(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Game Data Singleton")
	static UGameDataSingleton* GetGameDataSingleton();
};