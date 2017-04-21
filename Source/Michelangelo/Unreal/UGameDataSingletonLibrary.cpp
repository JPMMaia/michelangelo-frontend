#include "Michelangelo.h"
#include "UGameDataSingletonLibrary.h"
#include "../NonUnreal/Common/EngineException.h"

using namespace Common;

UGameDataSingletonLibrary::UGameDataSingletonLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGameDataSingleton* UGameDataSingletonLibrary::GetGameDataSingleton()
{
	auto dataInstance = Cast<UGameDataSingleton>(GEngine->GameSingleton);
	if (!dataInstance || !dataInstance->IsValidLowLevel())
		ThrowEngineException(L"Data instance isn't valid.");

	return dataInstance;
}