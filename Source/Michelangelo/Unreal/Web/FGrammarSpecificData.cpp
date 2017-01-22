#include "Michelangelo.h"
#include "FGrammarSpecificData.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "NonUnreal/MichelangeloAPI/GrammarSpecificData.h"

using namespace Common;

FGrammarSpecificData::FGrammarSpecificData() :
	ID("Unknown"),
	Name("Unknown"),
	Type("Unkown"),
	Code("Unknown"),
	Shared(false),
	IsOwner(false)
{
}

FGrammarSpecificData FGrammarSpecificData::FromGrammarSpecificData(const MichelangeloAPI::GrammarSpecificData& other)
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
