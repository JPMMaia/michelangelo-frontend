#pragma once

#include "AInstancedStaticMeshActor.h"
#include "AInstancedStaticMeshActorManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AInstancedStaticMeshActorManager : public AActor
{
	GENERATED_BODY()

public:
	explicit AInstancedStaticMeshActorManager(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "InstancecdStaticMeshActor")
	void AddInstancedStaticMeshActors(const FString& name, UStaticMesh* staticMesh);

	UFUNCTION(BlueprintCallable, Category = "InstancecdStaticMeshActor")
	AInstancedStaticMeshActor* GetInstancedStaticMeshActor(const FString& name);

	UPROPERTY(EditAnywhere, Category = "InstancecdStaticMeshActor")
	TMap<FString, AInstancedStaticMeshActor*> InstancedStaticMeshActors;
};