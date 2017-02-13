#pragma once

#include "GameFramework/HUD.h"
#include "StandardHUD.generated.h"

class SStandardWidget;

UCLASS()
class AStandardHUD : public AHUD
{
	GENERATED_BODY()

public:
	AStandardHUD();

	void BeginPlay();

private:
	TSharedPtr<SStandardWidget> m_myWidget;
};
