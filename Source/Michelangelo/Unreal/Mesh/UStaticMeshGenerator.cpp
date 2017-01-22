#include "Michelangelo.h"
#include "UStaticMeshGenerator.h"

UStaticMeshGenerator::UStaticMeshGenerator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StaticMeshes.Add(TEXT("Box"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object);
	StaticMeshes.Add(TEXT("Sphere"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object);
	StaticMeshes.Add(TEXT("Cylinder"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder")).Object);
	StaticMeshes.Add(TEXT("Cone"), ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cone")).Object);
}

UStaticMesh* UStaticMeshGenerator::GetStaticMesh(const FString& name)
{
	return *StaticMeshes.Find(name);
}
