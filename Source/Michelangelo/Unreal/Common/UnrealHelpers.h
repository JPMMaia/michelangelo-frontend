#pragma once

#include <array>
#include <string>

namespace Common
{
	namespace Helpers
	{
		void DestroyObject(UObject* objectToDestroy);
		std::string FStringToString(const FString& fstr);
		FString WStringToFString(const std::wstring& wstr);
		std::wstring FStringToWString(const FString& fstr);
		FString StringToFString(const std::string& str);
		FMatrix ArrayToMatrix(const std::array<float, 16>& transformArray);
		FMatrix MichelangeloToUnrealGeneralTransform(const FMatrix& transform);
		FMatrix MichelangeloToUnrealPrimitiveTransform(const FMatrix& transform);
		int32 GetObjectReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects = nullptr);
	}
}