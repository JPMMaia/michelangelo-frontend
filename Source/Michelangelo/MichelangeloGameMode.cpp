// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MichelangeloGameMode.h"
#include "Unreal/UGameDataSingleton.h"

AMichelangeloGameMode::AMichelangeloGameMode()
{
}

void AMichelangeloGameMode::StartPlay()
{
	UGameDataSingleton::Get()->SetSpawner(this);
	Super::StartPlay();
}
