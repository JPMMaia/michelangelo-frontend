#pragma once

#include <GameFramework/Actor.h>
#include "AAxisActor.generated.h"

UCLASS()
class MICHELANGELO_API AAxisActor : public AActor
{
	GENERATED_BODY()

public:
	explicit AAxisActor(const FObjectInitializer& objectInitializer);

	void BeginPlay() override;
};
