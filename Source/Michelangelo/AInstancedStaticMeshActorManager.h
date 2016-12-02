#pragma once

#include "AInstancedStaticMeshActor.h"
#include "AInstancedProceduralMeshActor.h"
#include "MichelangeloAPI/ObjectGeometry.h"
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