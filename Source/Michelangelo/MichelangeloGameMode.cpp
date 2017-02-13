// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MichelangeloGameMode.h"
#include "Unreal/UGameDataSingleton.h"
#include "Unreal/UI/StandardHUD.h"

AMichelangeloGameMode::AMichelangeloGameMode()
{
	// Set the hud class to use our custom HUD by default:
	HUDClass = AStandardHUD::StaticClass();
}

void AMichelangeloGameMode::StartPlay()
{
	UGameDataSingleton::Get()->SetSpawner(this);

	Super::StartPlay();
}
