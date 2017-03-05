#include "Michelangelo.h"
#include "FGrammarSpecificData.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"

using namespace Common;
using namespace MichelangeloAPI;

FGrammarSpecificData::FGrammarSpecificData() :
	ID("Unknown"),
	Name("Unknown"),
	Type("Unkown"),
	Code("Unknown"),
	Shared(false),
	IsOwner(false)
{
}

MichelangeloAPI::GrammarSpecificData FGrammarSpecificData::ToApiData() const
{
	GrammarSpecificData apiData;
	apiData.ID = Helpers::FStringToString(ID);
	apiData.Name = Helpers::FStringToString(Name);
	apiData.Type = Helpers::FStringToString(Type);
	apiData.Code = Helpers::FStringToWString(Code);
	apiData.Shared = Shared;
	apiData.IsOwner = IsOwner;
	return apiData;
}

FGrammarSpecificData FGrammarSpecificData::FromApiData(const MichelangeloAPI::GrammarSpecificData& other)
{
	FGrammarSpecificData object;
	object.ID = Helpers::StringToFString(other.ID);
	object.Name = Helpers::StringToFString(other.Name);
	object.Type = Helpers::StringToFString(other.Type);
	object.Code = Helpers::WStringToFString(other.Code);
	object.Shared = other.Shared;
	object.IsOwner = other.IsOwner;
	return object;
}
