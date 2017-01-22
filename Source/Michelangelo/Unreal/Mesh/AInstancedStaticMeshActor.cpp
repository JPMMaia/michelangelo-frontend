#include "Michelangelo.h"
#include "AInstancedStaticMeshActor.h"

AInstancedStaticMeshActor::AInstancedStaticMeshActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	// Create instanced static mesh component:
	InstancedStaticMeshComponent = objectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("InstancedStaticMeshComponent"));
	
	// Set it as root component:
	RootComponent = InstancedStaticMeshComponent;

	PrimaryActorTick.bCanEverTick = true;
}
