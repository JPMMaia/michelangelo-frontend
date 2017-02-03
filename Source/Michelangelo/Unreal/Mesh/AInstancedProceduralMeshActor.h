#pragma once

#include <GameFramework/Actor.h>
#include <ProceduralMeshComponent.h>

#include "AMeshActor.h"
#include "AInstancedProceduralMeshActor.generated.h"

namespace MichelangeloAPI 
{
	class ObjectGeometry;
}

UCLASS()
class MICHELANGELO_API AInstancedProceduralMeshActor : public AMeshActor
{
	GENERATED_BODY()

public:
	static AInstancedProceduralMeshActor* CreateFromGeometry(const MichelangeloAPI::ObjectGeometry& geometry);

public:
	explicit AInstancedProceduralMeshActor(const FObjectInitializer& objectInitializer);

	void AddInstance(const MichelangeloAPI::ObjectGeometry& instanceData) override;
	UMaterialInstanceDynamic* CreateDynamicMaterialInstance() override;

private:
	void CreateMeshSection(const MichelangeloAPI::ObjectGeometry& geometry);

private:
	static size_t s_id;

	UPROPERTY()
	UProceduralMeshComponent* m_proceduralMeshComponent;

	int32 m_sectionCount;
};
