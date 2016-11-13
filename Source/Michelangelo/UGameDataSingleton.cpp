#include "Michelangelo.h"
#include "UGameDataSingleton.h"

UGameDataSingleton::UGameDataSingleton(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	StaticMeshGenerator(nullptr),
	InstancedStaticMeshActorManager(nullptr)
{
}

UStaticMeshGenerator* UGameDataSingleton::GetStaticMeshGenerator()
{
	if (!this->StaticMeshGenerator)
		this->StaticMeshGenerator = NewObject<UStaticMeshGenerator>();

	return this->StaticMeshGenerator;
}

AInstancedStaticMeshActorManager* UGameDataSingleton::GetInstancedStaticMeshActorManager(UWorld* world)
{
	if (!this->InstancedStaticMeshActorManager)
		this->InstancedStaticMeshActorManager = world->SpawnActor<AInstancedStaticMeshActorManager>(AInstancedStaticMeshActorManager::StaticClass());

	return this->InstancedStaticMeshActorManager;
}
