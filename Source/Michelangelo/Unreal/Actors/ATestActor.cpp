#include "Michelangelo.h"
#include "Unreal/Actors/ATestActor.h"

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
	auto x = FString::SanitizeFloat(GNearClippingPlane);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%s"), *x));
	
}
