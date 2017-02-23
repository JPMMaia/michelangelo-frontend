#include "Michelangelo.h"
#include "InstancedStaticSkewedMeshComponent.h"
#include "PhysXIncludes.h"
#include "Private/InstancedStaticMesh.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"

int32 UInstancedStaticSkewedMeshComponent::AddInstanceMatrix(const FMatrix& InInstanceTransform)
{
	auto InstanceIdx = PerInstanceSMData.Num();

	auto NewInstanceData = new (PerInstanceSMData) FInstancedStaticMeshInstanceData();
	SetupNewInstanceDataMatrix(*NewInstanceData, InstanceIdx, InInstanceTransform);

#if WITH_EDITOR
	if (SelectedInstances.Num())
	{
		SelectedInstances.Add(false);
	}
#endif

	ReleasePerInstanceRenderData();
	MarkRenderStateDirty();

	PartialNavigationUpdate(InstanceIdx);

	return InstanceIdx;
}
int32 UInstancedStaticSkewedMeshComponent::AddInstanceWorldSpaceMatrix(const FMatrix& InWorldTransform)
{
	// TODO Transform from world space to local space:
	auto LocalTransform = InWorldTransform;
	return AddInstanceMatrix(LocalTransform);
}

void UInstancedStaticSkewedMeshComponent::SetupNewInstanceDataMatrix(FInstancedStaticMeshInstanceData& InOutNewInstanceData, int32 InInstanceIndex, const FMatrix& InInstanceTransform)
{
	InOutNewInstanceData.Transform = InInstanceTransform;
	InOutNewInstanceData.LightmapUVBias = FVector2D(-1.0f, -1.0f);
	InOutNewInstanceData.ShadowmapUVBias = FVector2D(-1.0f, -1.0f);

	if (bPhysicsStateCreated)
	{
		// Add another aggregate if needed
		// Aggregates aren't used for static objects
		if (Mobility == EComponentMobility::Movable)
		{
			const int32 NumShapes = GetNumShapes(GetBodySetup());
			const int32 AggregateIndex = GetAggregateIndex(InInstanceIndex, NumShapes);
			if (AggregateIndex >= Aggregates.Num())
			{
				// Get the scene type from the main BodyInstance
				FPhysScene* PhysScene = GetWorld()->GetPhysicsScene();
				check(PhysScene);

				const uint32 SceneType = BodyInstance.UseAsyncScene(PhysScene) ? PST_Async : PST_Sync;

				auto* Aggregate = GPhysXSDK->createAggregate(AggregateMaxSize, false);
				const int32 AddedIndex = Aggregates.Add(Aggregate);
				check(AddedIndex == AggregateIndex);
				PxScene* PScene = PhysScene->GetPhysXScene(SceneType);
				SCOPED_SCENE_WRITE_LOCK(PScene);
				PScene->addAggregate(*Aggregate);
			}
		}

		if (InInstanceTransform.GetScaleVector().IsNearlyZero())
		{
			InstanceBodies.Insert(nullptr, InInstanceIndex);
		}
		else
		{
			FBodyInstance* NewBodyInstance = new FBodyInstance();
			int32 BodyIndex = InstanceBodies.Insert(NewBodyInstance, InInstanceIndex);
			check(InInstanceIndex == BodyIndex);
			InitInstanceBody(BodyIndex, NewBodyInstance);
		}
	}
}

int32 UInstancedStaticSkewedMeshComponent::GetNumShapes(UBodySetup* BodySetup)
{
	int32 NumShapes = 1;
	if (BodySetup)
	{
		NumShapes = FMath::Max(BodySetup->AggGeom.GetElementCount(), NumShapes);	//if there's no simple shapes we still have a trimesh so 1 is the min
	}

	return NumShapes;
}
int32 UInstancedStaticSkewedMeshComponent::GetAggregateIndex(int32 BodyIndex, int32 NumShapes)
{
	const int32 BodiesPerBucket = AggregateMaxSize / NumShapes;
	return BodyIndex / BodiesPerBucket;
}
