#pragma once

#include "UStaticMeshGenerator.h"
#include "AInstancedStaticMeshActorManager.h"
#include "UGameDataSingleton.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UGameDataSingleton : public UObject
{
	GENERATED_BODY()
public:
	explicit UGameDataSingleton(const FObjectInitializer& ObjectInitializer);

	UStaticMeshGenerator* GetStaticMeshGenerator();
	AInstancedStaticMeshActorManager* GetInstancedStaticMeshActorManager(UWorld* world);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data Singleton")
	UStaticMeshGenerator* StaticMeshGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data Singleton")
	AInstancedStaticMeshActorManager* InstancedStaticMeshActorManager;
};