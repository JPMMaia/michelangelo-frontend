#include "Michelangelo.h"
#include "AInstancedStaticMeshActorManager.h"
#include "UGameDataSingletonLibrary.h"
#include "Common/Helpers.h"

using namespace Common;

AInstancedStaticMeshActorManager::AInstancedStaticMeshActorManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AInstancedStaticMeshActorManager::BeginPlay()
{
	Super::BeginPlay();

	// Get game data singleton:
	auto gameData = UGameDataSingletonLibrary::GetGameDataSingleton();

	// Generate default instanced static mesh actors for basic primitives:
	auto staticMeshGenerator = gameData->GetStaticMeshGenerator();
	AddInstancedStaticMeshActors("Box", staticMeshGenerator->CubeStaticMesh);
	AddInstancedStaticMeshActors("Sphere", staticMeshGenerator->SphereStaticMesh);
	AddInstancedStaticMeshActors("Cylinder", staticMeshGenerator->CylinderStaticMesh);
	AddInstancedStaticMeshActors("Cone", staticMeshGenerator->ConeStaticMesh);
}

void AInstancedStaticMeshActorManager::AddInstancedStaticMeshActors(const FString& name, UStaticMesh* staticMesh)
{
	// If the key already exists:
	if (this->InstancedStaticMeshActors.Find(name) != nullptr)
		ThrowEngineException(L"Key already exists.");

	FActorSpawnParameters parameters;
	parameters.Name = FName(*name);
	parameters.Owner = this;
	auto actor = GetWorld()->SpawnActor<AInstancedStaticMeshActor>(AInstancedStaticMeshActor::StaticClass(), parameters);
	actor->InstancedStaticMeshComponent->SetStaticMesh(staticMesh);

	this->InstancedStaticMeshActors.Add(name, actor);
}

AInstancedStaticMeshActor* AInstancedStaticMeshActorManager::GetInstancedStaticMeshActor(const FString& name)
{
	auto location = this->InstancedStaticMeshActors.Find(name);
	if (location == nullptr)
		return nullptr;

	return *location;
}
