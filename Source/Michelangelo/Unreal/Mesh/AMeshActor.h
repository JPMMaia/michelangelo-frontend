#pragma once

#include <GameFramework/Actor.h>
#include "AInstancedStaticMeshActor.generated.h"

namespace MichelangeloAPI
{
	class ObjectGeometry;
}

UCLASS()
class MICHELANGELO_API AMeshActor : public AActor
{
	GENERATED_BODY()

public:
	explicit AMeshActor(const FObjectInitializer& objectInitializer) :
		Super(objectInitializer)
	{
	}

	virtual void AddInstance(const MichelangeloAPI::ObjectGeometry& instanceData) = 0;
};
