#pragma once

#include <UObject.h>

#include "URenderItem.h"
#include "Unreal/Mesh/AInstancedStaticMeshActor.h"
#include "Unreal/Mesh/AInstancedProceduralMeshActor.h"

#include "URenderItemsCollection.generated.h"

namespace MichelangeloAPI 
{
	class SceneGeometry;
}

UCLASS()
class MICHELANGELO_API URenderItemsCollection : public UObject
{
	GENERATED_BODY()
public:
	void AddGeometry(const MichelangeloAPI::SceneGeometry& sceneGeometry);

private:
	void HandleMaterials(const MichelangeloAPI::SceneGeometry& sceneGeometry);
	void HandleObjects(const MichelangeloAPI::SceneGeometry& sceneGeometry);
	
	void CreateStaticMesh(const MichelangeloAPI::ObjectGeometry& objectGeometry);
	void CreateProceduralMesh(const MichelangeloAPI::ObjectGeometry& objectGeometry);

private:
	TSet<URenderItem> m_renderItems;
	TMap<FString, UMaterialInterface*> m_materials;
	TMap<FString, AInstancedStaticMeshActor*> m_staticMeshActors;
	TArray<AInstancedProceduralMeshActor*> m_proceduralMeshActors;
};