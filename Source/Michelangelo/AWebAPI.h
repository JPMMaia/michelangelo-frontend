// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MichelangeloAPI/WebAPI.h"
#include "AWebAPI.generated.h"

UCLASS()
class MICHELANGELO_API AAWebAPI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWebAPI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category="WebAPI")
	bool Authenticate(const FString& email, const FString& password, bool rememberMe);

	UFUNCTION(BlueprintCallable, Category = "Test")
	void ToggleLight();

private:
	MichelangeloAPI::WebAPI m_webAPI;
};
