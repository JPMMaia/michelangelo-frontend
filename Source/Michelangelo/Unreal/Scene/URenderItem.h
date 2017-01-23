#pragma once

#include <UObject.h>
#include "URenderItem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class MICHELANGELO_API URenderItem : public UObject
{
	GENERATED_BODY()
public:


	friend bool operator==(const URenderItem& lhs, const URenderItem& rhs)
	{
		return false;
	}
	friend bool operator!=(const URenderItem& lhs, const URenderItem& rhs)
	{
		return !(lhs == rhs);
	}

private:
};
