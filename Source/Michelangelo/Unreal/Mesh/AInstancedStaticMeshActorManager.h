#pragma once

#include "Unreal/Mesh/AInstancedStaticMeshActor.h"
#include "Unreal/Mesh/AInstancedProceduralMeshActor.h"
#include "NonUnreal/MichelangeloAPI/ObjectGeometry.h"
#include "AInstancedStaticMeshActorManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AInstancedStaticMeshActorManager : public AActor
{
	GENERATED_BODY()

public:
	explicit AInstancedStaticMeshActorManager(const FObjectInitializer& ObjectInitializer);

	void AddGeometry(const MichelangeloAPI::ObjectGeometry& geometry);
	void AddStaticMeshGeometry(const MichelangeloAPI::ObjectGeometry& geometry);
	void AddProceduralMeshGeometry(const MichelangeloAPI::ObjectGeometry& geometry);

	UPROPERTY(EditAnywhere, Category = "InstancedMeshActorManager")
	TMap<FString, AInstancedStaticMeshActor*> InstancedStaticMeshActors;

	UPROPERTY(EditAnywhere, Category = "InstancedMeshActorManager")
	TMap<FString, AInstancedProceduralMeshActor*> InstancedProceduralMeshActors;
};