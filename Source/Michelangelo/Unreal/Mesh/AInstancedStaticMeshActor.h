#pragma once

#include "AMeshActor.h"

#include <Components/InstancedStaticMeshComponent.h>

#include "AInstancedStaticMeshActor.generated.h"

UCLASS()
class MICHELANGELO_API AInstancedStaticMeshActor : public AMeshActor
{
	GENERATED_BODY()

public:
	static AInstancedStaticMeshActor* CreateFromGeometry(const MichelangeloAPI::ObjectGeometry& geometry);

public:
	explicit AInstancedStaticMeshActor(const FObjectInitializer& objectInitializer);

	void AddInstance(const MichelangeloAPI::ObjectGeometry& instanceData) override;

private:
	UInstancedStaticMeshComponent* m_instancedStaticMeshComponent;
};
