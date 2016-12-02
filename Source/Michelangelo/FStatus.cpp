// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "FStatus.h"

FStatus::FStatus() :
	Success(true),
	Message(TEXT(""))
{
}

FStatus::FStatus(bool success, const FString& message) :
	Success(success),
	Message(message)
{
}
