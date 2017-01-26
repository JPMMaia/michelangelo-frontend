#pragma once

#include <UObject.h>

#include "Unreal/Mesh/AMeshActor.h"
#include "URenderItem.generated.h"

namespace MichelangeloAPI 
{
	class SceneGeometry;
}

UCLASS()
class MICHELANGELO_API URenderItem : public UObject
{
	GENERATED_BODY()
public:
	static URenderItem* Create(const MichelangeloAPI::SceneGeometry& sceneGeometry, const MichelangeloAPI::ObjectGeometry& objectGeometry);

public:
	AMeshActor* GetMeshActor() const;
	UMaterialInterface* GetMaterial() const;
	const FString& GetMeshName() const;
	size_t GetMaterialIndex() const;

private:
	AMeshActor* m_meshActor = nullptr;
	UMaterialInterface* m_material = nullptr;
	FString m_meshName;
	size_t m_materialIndex;
};
