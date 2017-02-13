#include "Michelangelo.h"
#include "StandardWidget.h"
#include "StandardHUD.h"

AStandardHUD::AStandardHUD()
{
}

void AStandardHUD::BeginPlay()
{
	// Create widget:
	m_myWidget = SNew(SStandardWidget).m_ownerHUD(this);

	// Pass weak pointer to viewport:
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_myWidget.ToSharedRef()));

	// Set widget to visible:
	m_myWidget->SetVisibility(EVisibility::Visible);
}