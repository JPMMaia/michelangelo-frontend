#include "Michelangelo.h"
#include "AInstancedStaticMeshActor.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"

using namespace Common;

AInstancedStaticMeshActor* AInstancedStaticMeshActor::CreateFromGeometry(const MichelangeloAPI::ObjectGeometry& geometry)
{
	auto gameDataSingleton = UGameDataSingletonLibrary::GetGameDataSingleton();

	// Get spawner:
	auto spawner = gameDataSingleton->GetSpawner();

	// Get name of geometry:
	auto name = Common::Helpers::StringToFString(geometry.GetName());

	// Instantiate actor:
	FActorSpawnParameters parameters;
	parameters.Name = FName(*name);
	parameters.Owner = spawner;
	auto actor = spawner->GetWorld()->SpawnActor<AInstancedStaticMeshActor>(AInstancedStaticMeshActor::StaticClass(), parameters);

	// Set static mesh:
	auto staticMeshGenerator = gameDataSingleton->GetStaticMeshLoader();
	actor->m_instancedStaticMeshComponent->SetStaticMesh(staticMeshGenerator->GetStaticMesh(name));

	m_instancedStaticMeshComponent->CreateDynamicMaterialInstance(0);

	return actor;
}

AInstancedStaticMeshActor::AInstancedStaticMeshActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	// Create instanced static mesh component:
	m_instancedStaticMeshComponent = objectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("InstancedStaticMeshComponent"));
	
	// Set it as root component:
	RootComponent = m_instancedStaticMeshComponent;

	PrimaryActorTick.bCanEverTick = true;
}

void AInstancedStaticMeshActor::AddInstance(const MichelangeloAPI::ObjectGeometry& instanceData)
{
	// Create world transform:
	auto matrix = Helpers::ArrayToMatrix(instanceData.GetTransform());
	matrix = Helpers::MichelangeloToUnrealPrimitiveTransform(matrix);
	auto worldTransform = FTransform(matrix);

	// Add instance:
	m_instancedStaticMeshComponent->AddInstanceWorldSpace(worldTransform);
}
