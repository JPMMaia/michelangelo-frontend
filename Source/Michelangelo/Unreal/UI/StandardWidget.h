#pragma once
#include "StandardHUD.h"
#include "SlateBasics.h"

class SStandardWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SStandardWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AStandardHUD>, m_ownerHUD)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TWeakObjectPtr<class AStandardHUD> m_ownerHUD;
};