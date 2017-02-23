#include "Michelangelo.h"
#include "URenderItemsCollection.h"
#include "NonUnreal/MichelangeloAPI/SceneGeometry.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "Unreal/UGameDataSingleton.h"

using namespace Common;
using namespace MichelangeloAPI;

void URenderItemsCollection::Clear()
{
	for (auto pRenderItem : m_renderItems)
		Helpers::DestroyObject(pRenderItem);
	UGameDataSingleton::Get()->GetSpawner()->GetWorld()->ForceGarbageCollection(true);

	m_renderItems.clear();
}

void URenderItemsCollection::AddGeometry(const SceneGeometry& sceneGeometry)
{
	for (const auto& object : sceneGeometry.GetObjects())
	{
		// Build search function:
		auto matchFunction = [&object](URenderItem* other)
		{
			return
				other->GetMeshName() == Helpers::StringToFString(object.GetName()) &&
				other->GetMaterialIndex() == object.GetMaterialIndex();
		};

		// Find a match for the object geometry:
		auto match = std::find_if(m_renderItems.begin(), m_renderItems.end(), matchFunction);

		// If a match is not found:
		if (match == m_renderItems.end())
		{
			// Create new render item:
			m_renderItems.insert(URenderItem::Create(sceneGeometry, object));
		}
		// If found:
		else
		{
			// Add new instance:
			(*match)->GetMeshActor()->AddInstance(object);
		}
	}
}
