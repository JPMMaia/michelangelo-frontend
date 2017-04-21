#include "Michelangelo.h"
#include "StaticMeshLoader.h"

UStaticMeshLoader::UStaticMeshLoader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StaticMeshes.Add(TEXT("Box"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object);
	StaticMeshes.Add(TEXT("Sphere"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object);
	StaticMeshes.Add(TEXT("Cylinder"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder")).Object);
	StaticMeshes.Add(TEXT("Cone"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cone")).Object);
}

UStaticMesh* UStaticMeshLoader::GetStaticMesh(const FString& name)
{
	return *StaticMeshes.Find(name);
}
