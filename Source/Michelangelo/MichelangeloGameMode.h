// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MichelangeloGameMode.generated.h"

/**
 *
 */
UCLASS()
class MICHELANGELO_API AMichelangeloGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMichelangeloGameMode();

	void StartPlay() override;
};
