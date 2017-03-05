#pragma once

#include <unordered_set>
#include <UObject.h>

#include "URenderItem.h"

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
	UFUNCTION(BlueprintCallable, Category = "Scene")
	void Clear();

	void AddGeometry(const MichelangeloAPI::SceneGeometry& sceneGeometry);

private:
	std::unordered_set<URenderItem*> m_renderItems;
};