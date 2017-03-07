// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "GrammarListItem.h"
#include "Unreal/UGameDataSingleton.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"

#include <thread>
#include "Unreal/Common/UnrealHelpers.h"

using namespace Common;

void UGrammarListItem::ShareGrammar()
{
	// Lock data mutex:
	std::lock_guard<std::mutex> lock(m_dataMutex);

	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	// Invert share value:
	GrammarSpecificData->Shared = !GrammarSpecificData->Shared;

	// Make request to inform of the value change:
	nativeWebAPI.ShareGrammar(Helpers::FStringToString(GrammarSpecificData->ID), GrammarSpecificData->Shared);
}
void UGrammarListItem::ShareGrammarAsync()
{
	auto async = std::bind(&UGrammarListItem::ShareGrammar, this);
	std::thread(async).detach();
}

void UGrammarListItem::DeleteGrammar()
{
	// Lock data mutex:
	std::lock_guard<std::mutex> lock(m_dataMutex);

	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();
	nativeWebAPI.DeleteGrammar(Helpers::FStringToString(GrammarSpecificData->ID));
}
void UGrammarListItem::DeleteGrammarAsync()
{
	auto async = std::bind(&UGrammarListItem::DeleteGrammar, this);
	std::thread(async).detach();
}

void UGrammarListItem::SetData(UGrammarSpecificData* data)
{
	GrammarSpecificData = data;
}
