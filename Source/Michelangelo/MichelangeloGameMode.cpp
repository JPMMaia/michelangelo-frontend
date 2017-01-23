// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "MichelangeloGameMode.h"
#include "Unreal/UGameDataSingletonLibrary.h"

AMichelangeloGameMode::AMichelangeloGameMode()
{
	UGameDataSingletonLibrary::GetGameDataSingleton()->SetSpawner(this);
}
