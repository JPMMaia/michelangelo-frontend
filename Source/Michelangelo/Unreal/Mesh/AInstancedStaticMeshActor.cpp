#include "Michelangelo.h"
#include "AInstancedStaticMeshActor.h"
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/ObjectGeometry.h"

using namespace Common;
using namespace MichelangeloAPI;

size_t AInstancedStaticMeshActor::s_id = 0;

AInstancedStaticMeshActor* AInstancedStaticMeshActor::CreateFromGeometry(const MichelangeloAPI::ObjectGeometry& geometry)
{
	auto gameDataSingleton = UGameDataSingleton::Get();

	// Get spawner:
	auto spawner = gameDataSingleton->GetSpawner();

	// Get name of geometry:
	auto staticMeshName = Helpers::StringToFString(geometry.GetName());

	// Instantiate actor:
	FActorSpawnParameters parameters;
	auto uniqueName = staticMeshName;
	uniqueName.AppendInt(s_id++);
	parameters.Name = FName(*uniqueName);
	parameters.Owner = spawner;
	auto actor = spawner->GetWorld()->SpawnActor<AInstancedStaticMeshActor>(AInstancedStaticMeshActor::StaticClass(), parameters);

	// Set static mesh:
	auto staticMeshGenerator = gameDataSingleton->GetStaticMeshLoader();
	actor->m_instancedStaticMeshComponent->SetStaticMesh(staticMeshGenerator->GetStaticMesh(staticMeshName));
	
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
UMaterialInstanceDynamic* AInstancedStaticMeshActor::CreateDynamicMaterialInstance()
{
	// Get a source material loaded from a blueprint:
	auto materialLoader = UGameDataSingleton::Get()->GetMaterialLoader();
	auto sourceMaterial = materialLoader->GetMaterial("BasicMaterial");

	// Create material instance:
	return m_instancedStaticMeshComponent->CreateDynamicMaterialInstance(0, sourceMaterial);
}
