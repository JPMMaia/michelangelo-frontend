#pragma once

#include <GameFramework/Actor.h>
#include "AMeshActor.generated.h"

namespace MichelangeloAPI
{
	class ObjectGeometry;
}

UCLASS()
class MICHELANGELO_API AMeshActor : public AActor
{
	GENERATED_BODY()

public:
	virtual void AddInstance(const MichelangeloAPI::ObjectGeometry& instanceData);
	virtual UMaterialInstanceDynamic* CreateDynamicMaterialInstance();
};
