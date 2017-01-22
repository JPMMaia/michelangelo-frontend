#pragma once

#include <GameFramework/Actor.h>
#include <Components/InstancedStaticMeshComponent.h>
#include "AInstancedStaticMeshActor.generated.h"

UCLASS()
class MICHELANGELO_API AInstancedStaticMeshActor : public AActor
{
	GENERATED_BODY()

public:
	explicit AInstancedStaticMeshActor(const FObjectInitializer& objectInitializer);

	UPROPERTY(EditAnywhere, Category = StaticMeshAssets)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
};
