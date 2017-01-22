#pragma once

#include <GameFramework/Actor.h>
#include "ATestActor.generated.h"

UCLASS()
class MICHELANGELO_API ATestActor : public AActor
{
	GENERATED_BODY()

public:
	explicit ATestActor(const FObjectInitializer& objectInitializer);

	void BeginPlay() override;
};
