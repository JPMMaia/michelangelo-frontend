#include "Michelangelo.h"
#include "UMaterialLoader.h"

UMaterialLoader::UMaterialLoader(const FObjectInitializer& ObjectInitializer)
{
	AddMaterial("BasicMaterial");
}

UMaterialInterface* UMaterialLoader::GetMaterial(const FString& name)
{
	return Materials[name];
}

void UMaterialLoader::AddMaterial(const FString& name)
{
	auto materialPath = "Material'/Game/Materials/" + name + "." + name + "'";
	auto charArray = materialPath.GetCharArray();
	ConstructorHelpers::FObjectFinder<UMaterial> material(charArray.GetData());
	if (!material.Succeeded())
		return;

	Materials.Add(name, material.Object);	
}
