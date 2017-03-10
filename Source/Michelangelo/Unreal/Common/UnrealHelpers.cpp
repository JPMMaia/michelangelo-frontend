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
	return FMatrix(
	FPlane(transformArray[0], transformArray[4], transformArray[8], transformArray[12]),
	FPlane(transformArray[1], transformArray[5], transformArray[9], transformArray[13]),
	FPlane(transformArray[2], transformArray[6], transformArray[10], transformArray[14]),
	FPlane(transformArray[3], transformArray[7], transformArray[11], transformArray[15])
	);
}

const FMatrix& Helpers::GetUnrealToMichelangeloMatrix()
{
	static FMatrix unrealToMichelangelo(
		FPlane(0.0f, 1.0f, 0.0f, 0.0f),
		FPlane(1.0f, 0.0f, 0.0f, 0.0f),
		FPlane(0.0f, 0.0f, 1.0f, 0.0f),
		FPlane(0.0f, 0.0f, 0.0f, 1.0f)
	);

	return unrealToMichelangelo;
}

FMatrix Helpers::MichelangeloToUnrealGeneralTransform(const FMatrix& transform)
{
	const auto& unrealToMichelangelo = GetUnrealToMichelangeloMatrix();
	auto michelangeloToUnreal = unrealToMichelangelo.Inverse();

	auto modelMatrix = FRotationMatrix(FRotator(180.0f, 0.0f, 0.0f)) * FRotationMatrix(FRotator(0.0f, -90.0f, 0.0f));
	auto output = unrealToMichelangelo * transform * modelMatrix * michelangeloToUnreal;

	constexpr auto translationScalar = 100.0f;
	output.ScaleTranslation(FVector(translationScalar, translationScalar, translationScalar));

	auto determinant = output.Determinant();
	if (determinant < 0.0f)
		output = FScaleMatrix(-1.0f) * output;

	return output;
}
FMatrix Helpers::MichelangeloToUnrealPrimitiveTransform(const FMatrix& transform)
{
	const auto& unrealToMichelangelo = GetUnrealToMichelangeloMatrix();
	auto michelangeloToUnreal = unrealToMichelangelo.Inverse();
	auto modelMatrix = FRotationMatrix(FRotator(90.0f, 0.0f, 0.0f));

	auto output = modelMatrix * unrealToMichelangelo * transform * michelangeloToUnreal * modelMatrix;

	constexpr auto translationScalar = 100.0f;
	output.ScaleTranslation(FVector(translationScalar, translationScalar, translationScalar));

	auto determinant = output.Determinant();
	if (determinant < 0.0f)
		output = FScaleMatrix(-1.0f) * output;

	return output;
}

FVector Helpers::TransformPositionFromUnrealToMichelangeloSpace(const FVector& position)
{
	const auto& unrealToMichelangelo = GetUnrealToMichelangeloMatrix();
	return unrealToMichelangelo.TransformPosition(position) / 100.0f;
}
FVector Helpers::TransformVectorFromUnrealToMichelangeloSpace(const FVector& vector)
{
	const auto& unrealToMichelangelo = GetUnrealToMichelangeloMatrix();
	return unrealToMichelangelo.TransformVector(vector);
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

MichelangeloAPI::GrammarType Helpers::UnrealToNativeGrammarType(EGrammarType unrealGrammarType)
{
	switch(unrealGrammarType)
	{
	case EGrammarType::Own: return MichelangeloAPI::GrammarType::Own;
	case EGrammarType::Shared: return MichelangeloAPI::GrammarType::Shared;
	case EGrammarType::Tutorial: return MichelangeloAPI::GrammarType::Tutorial;
	default: return MichelangeloAPI::GrammarType::Unknown;
	}
}
EGrammarType Helpers::NativeToUnrealGrammarType(MichelangeloAPI::GrammarType nativeGrammarType)
{
	switch (nativeGrammarType)
	{
	case MichelangeloAPI::GrammarType::Own: return EGrammarType::Own;
	case MichelangeloAPI::GrammarType::Shared: return EGrammarType::Shared;
	case MichelangeloAPI::GrammarType::Tutorial: return EGrammarType::Tutorial;
	default: return EGrammarType::Unknown;
	}
}
