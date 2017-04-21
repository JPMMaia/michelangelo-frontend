#include "Michelangelo.h"
#include "GrammarSpecificData.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"

using namespace Common;
using namespace MichelangeloAPI;

UGrammarSpecificData::UGrammarSpecificData() :
	ID("Unknown"),
	Name("Unknown"),
	Type("Unknown"),
	Code("Unknown"),
	Shared(false),
	IsOwner(false),
	GrammarType(EGrammarType::Unknown),
	LastModified()
{
}

MichelangeloAPI::GrammarSpecificData UGrammarSpecificData::ToNativeData() const
{
	GrammarSpecificData apiData;
	apiData.ID = Helpers::FStringToString(ID);
	apiData.Name = Helpers::FStringToString(Name);
	apiData.Type = Helpers::FStringToString(Type);
	apiData.Code = Helpers::FStringToWString(Code);
	apiData.Shared = Shared;
	apiData.IsOwner = IsOwner;
	apiData.GrammarType = Helpers::UnrealToNativeGrammarType(GrammarType);
	apiData.LastModified = Helpers::DateTimeToTimePoint(LastModified);
	return apiData;
}

UGrammarSpecificData* UGrammarSpecificData::FromNativeData(const MichelangeloAPI::GrammarSpecificData& other)
{
	UGrammarSpecificData* object = NewObject<UGrammarSpecificData>();
	object->ID = Helpers::StringToFString(other.ID);
	object->Name = Helpers::StringToFString(other.Name);
	object->Type = Helpers::StringToFString(other.Type);
	object->Code = Helpers::WStringToFString(other.Code);
	object->Shared = other.Shared;
	object->IsOwner = other.IsOwner;
	object->GrammarType = Helpers::NativeToUnrealGrammarType(other.GrammarType);
	object->LastModified = Helpers::TimePointToDateTime(other.LastModified);
	return object;
}
