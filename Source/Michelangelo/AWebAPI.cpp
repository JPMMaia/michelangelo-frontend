// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "AWebAPI.h"
#include "Common/Helpers.h"

using namespace Common;

// Sets default values
AAWebAPI::AAWebAPI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAWebAPI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWebAPI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool AAWebAPI::Authenticate(const FString& email, const FString& password, bool rememberMe)
{
	return m_webAPI.Authenticate(Helpers::FStringToString(email), Helpers::FStringToString(password), rememberMe);
}
