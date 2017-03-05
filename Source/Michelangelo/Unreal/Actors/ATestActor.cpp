#include "Michelangelo.h"
#include "Unreal/Actors/ATestActor.h"
#include "Unreal/Web/AWebAPI.h"

#include <fstream>
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"

using namespace std;

ATestActor::ATestActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	// Create a default root component:
	RootComponent = objectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RootComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void ATestActor::BeginPlay()
{
}
