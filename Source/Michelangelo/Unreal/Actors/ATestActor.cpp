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
/*	// Read geometry from file:
	MichelangeloAPI::SceneGeometry sceneGeometry;
	{
		ifstream fileStream(L"TestMesh.json", ios::in);
		nlohmann::json json;
		fileStream >> json;
		sceneGeometry = MichelangeloAPI::SceneGeometry::CreateFromJson(json);
	}

	auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();;
	renderItems->Clear();
	renderItems->AddGeometry(sceneGeometry);*/
}
