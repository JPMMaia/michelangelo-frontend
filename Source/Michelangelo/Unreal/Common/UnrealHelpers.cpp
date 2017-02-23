#include "Michelangelo.h"
#include "UnrealHelpers.h"
#include "NonUnreal/Common/Helpers.h"

using namespace Common;

void Helpers::DestroyObject(UObject* objectToDestroy)
{
	if (!objectToDestroy || !objectToDestroy->IsValidLowLevel())
		return;

	// Destroy object:
	{
		if (objectToDestroy->IsRooted())
			objectToDestroy->RemoveFromRoot();

		auto pActor = Cast<AActor>(objectToDestroy);
		if (pActor)
			pActor->K2_DestroyActor();
		else
			objectToDestroy->ConditionalBeginDestroy();
	}
}

std::string Helpers::FStringToString(const FString& fstr)
{
	return std::string(TCHAR_TO_UTF8(*fstr));
}
FString Helpers::WStringToFString(const std::wstring& wstr)
{
	return StringToFString(WStringToString(wstr));
}
std::wstring Helpers::FStringToWString(const FString& fstr)
{
	return StringToWString(FStringToString(fstr));
}
FString Helpers::StringToFString(const std::string& str)
{
	return FString(str.c_str());
}
FMatrix Helpers::ArrayToMatrix(const std::array<float, 16>& transformArray)
{
	/*return FMatrix(
		FPlane(transformArray[0], transformArray[1], transformArray[2], transformArray[3]),
		FPlane(transformArray[4], transformArray[5], transformArray[6], transformArray[7]),
		FPlane(transformArray[8], transformArray[9], transformArray[10], transformArray[11]),
		FPlane(transformArray[12], transformArray[13], transformArray[14], transformArray[15])
	);*/
	return FMatrix(
	FPlane(transformArray[0], transformArray[4], transformArray[8], transformArray[12]),
	FPlane(transformArray[1], transformArray[5], transformArray[9], transformArray[13]),
	FPlane(transformArray[2], transformArray[6], transformArray[10], transformArray[14]),
	FPlane(transformArray[3], transformArray[7], transformArray[11], transformArray[15])
	);
}

FMatrix Helpers::MichelangeloToUnrealGeneralTransform(const FMatrix& transform)
{
	/*auto output = transform * FMatrix(
		FPlane(0.0f, 0.0f, -1.0f, 0.0f),
		FPlane(1.0f, 0.0f, 0.0f, 0.0f),
		FPlane(0.0f, 1.0f, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 0.0f, 1.0f)
		);*/
	auto output = transform * FMatrix(
		FPlane(0.0f, 1.0f, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 1.0f, 0.0f),
		FPlane(-1.0f, 0.0f, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 0.0f, 1.0f)
	);
	output.ScaleTranslation(FVector(100.0f, 100.0f, 100.0f));
	return output;

	/*return transform * FMatrix(
		FPlane(0.0f, 100.0f, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 100.0f, 0.0f),
		FPlane(-100.0f, 0.0f, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 0.0f, 1.0f)
	);*/
}

FMatrix Helpers::MichelangeloToUnrealPrimitiveTransform(const FMatrix& transform)
{
	FTransform newTransform(transform);

	auto oldTranslation = newTransform.GetTranslation();
	auto newTranslation = 100.0f * FVector(-oldTranslation.Z, oldTranslation.X, oldTranslation.Y);
	newTransform.SetTranslation(newTranslation);
	
	auto oldRotation = FRotator(newTransform.GetRotation());
	auto newPitch = -oldRotation.Roll;
	auto newYaw = -oldRotation.Pitch;
	auto newRoll = -oldRotation.Yaw;
	newTransform.SetRotation(FQuat(FRotator(newPitch, newYaw, newRoll)));	

	auto oldScale = transform.GetScaleVector();
	auto newScaleX = oldScale.Z;
	auto newScaleY = oldScale.X;
	auto newScaleZ = oldScale.Y;
	newTransform.SetScale3D(FVector(newScaleX, newScaleY, newScaleZ));

	return newTransform.ToMatrixWithScale();
}

///
/// Author: Rama, https://wiki.unrealengine.com/Garbage_Collection_~_Count_References_To_Any_Object#Code
///
int32 Helpers::GetObjectReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects)
{
	if (!Obj || !Obj->IsValidLowLevelFast())
	{
		return -1;
	}

	TArray<UObject*> ReferredToObjects; //req outer, ignore archetype, recursive, ignore transient
	FReferenceFinder ObjectReferenceCollector(ReferredToObjects, Obj, false, true, true, false);
	ObjectReferenceCollector.FindReferences(Obj);

	if (OutReferredToObjects)
	{
		OutReferredToObjects->Append(ReferredToObjects);
	}
	return OutReferredToObjects->Num();
}
