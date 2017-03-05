#include "Michelangelo.h"
#include "Unreal/Actors/AAxisActor.h"
#include "Unreal/Web/AWebAPI.h"

#include <fstream>
#include "Unreal/UGameDataSingletonLibrary.h"
#include "Unreal/Common/UnrealHelpers.h"

using namespace std;

AAxisActor::AAxisActor(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	// Create a default root component:
	RootComponent = objectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RootComponent"));

	PrimaryActorTick.bCanEverTick = true;
}

void AAxisActor::BeginPlay()
{
	DrawDebugLine(
		GetWorld(),
		FVector(0.0f, 0.0f, 0.0f),
		FVector(0.0f, 100.0f, 0.0f),
		FColor(255, 0, 0),
		true, -1, 0,
		12.333
	);

	DrawDebugLine(
		GetWorld(),
		FVector(0.0f, 0.0f, 0.0f),
		FVector(0.0f, 0.0f, 100.0f),
		FColor(0, 255, 0),
		true, -1, 0,
		12.333
	);

	DrawDebugLine(
		GetWorld(),
		FVector(0.0f, 0.0f, 0.0f),
		FVector(-100.0f, 0.0f, 0.0f),
		FColor(0, 0, 255),
		true, -1, 0,
		12.333
	);
}
