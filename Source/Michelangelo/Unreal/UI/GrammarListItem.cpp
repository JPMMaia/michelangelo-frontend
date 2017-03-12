// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "GrammarListItem.h"
#include "Unreal/UGameDataSingleton.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"

#include <thread>
#include "Unreal/Common/UnrealHelpers.h"

using namespace Common;

namespace Events
{
	class OnGrammarSharedEvent : public Common::Event<UGrammarListItem>
	{
	public:
		explicit OnGrammarSharedEvent(bool success) :
			m_success(success)
		{
		}

		void Handle(UGrammarListItem& sender) override
		{
			sender.OnGrammarSharedEvent.Broadcast(m_success);
		}

	private:
		bool m_success;
	};

	class OnGrammarDeletedEvent : public Common::Event<UGrammarListItem>
	{
	public:
		explicit OnGrammarDeletedEvent(bool success) :
			m_success(success)
		{
		}

		void Handle(UGrammarListItem& sender) override
		{
			sender.OnGrammarDeletedEvent.Broadcast(m_success);
		}

	private:
		bool m_success;
	};
}

void UGrammarListItem::RequestShareGrammar()
{
	bool newShareValue;
	std::string grammarId;
	{
		// Lock data mutex:
		std::lock_guard<std::mutex> lock(m_grammarDataMutex);

		// Invert share value:
		newShareValue = !m_grammarData->Shared;
		m_grammarData->Shared = newShareValue;

		// Cache grammar ID in a local variable:
		grammarId = Helpers::FStringToString(m_grammarData->ID);
	}

	// Get native web api:
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	// Make request to inform of the value change:
	try
	{
		nativeWebAPI.ShareGrammar(grammarId, newShareValue);
	}
	catch(const std::exception& error)
	{
		SetErrorMessage(error.what());
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarSharedEvent>(false));
		return;
	}

	m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarSharedEvent>(true));
}
void UGrammarListItem::RequestShareGrammarAsync()
{
	auto async = std::bind(&UGrammarListItem::RequestShareGrammar, this);
	m_tasksComponent.Add(Task(async));
}

void UGrammarListItem::RequestDeleteGrammar()
{
	auto grammarId = Helpers::FStringToString(GetGrammarID());
	
	auto& nativeWebAPI = UGameDataSingleton::Get()->GetWebAPI();

	try
	{
		nativeWebAPI.DeleteGrammar(grammarId);
	}
	catch (const std::exception& error)
	{
		SetErrorMessage(error.what());
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarSharedEvent>(false));
		return;
	}

	m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarDeletedEvent>(true));
}
void UGrammarListItem::RequestDeleteGrammarAsync()
{
	auto async = std::bind(&UGrammarListItem::RequestDeleteGrammar, this);
	m_tasksComponent.Add(Task(async));
}

UGrammarSpecificData* UGrammarListItem::GetGrammarData()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData;
}

bool UGrammarListItem::IsGrammarOwner()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData->IsOwner;
}
bool UGrammarListItem::IsGrammarShared()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData->Shared;
}

FString UGrammarListItem::GetGrammarName()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData->Name;
}

void UGrammarListItem::SetGrammarData(UGrammarSpecificData* data)
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	m_grammarData = data;
	OnGrammarDataChanged.Broadcast();
}

FString UGrammarListItem::GetGrammarID()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData->ID;
}

FString UGrammarListItem::GetGrammarType()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData->Type;
}

FDateTime UGrammarListItem::GetGrammarLastModified()
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	return m_grammarData->LastModified;
}

FString UGrammarListItem::GetGrammarLastModifiedFormattedString()
{
	FDateTime lastModified;
	{
		std::lock_guard<std::mutex> lock(m_grammarDataMutex);
		lastModified = m_grammarData->LastModified;
	}

	return lastModified.ToString(TEXT("%Y/%m/%d %H:%M:%S"));
}

FString UGrammarListItem::GetErrorMessage()
{
	std::lock_guard<std::mutex> lock(m_errorMesssageMutex);
	return m_errorMessage;
}

void UGrammarListItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);
	
	m_eventsComponent.HandleEvents(*this);
	m_tasksComponent.Update();
}

void UGrammarListItem::SetErrorMessage(const FString& errorMessage)
{
	std::lock_guard<std::mutex> lock(m_errorMesssageMutex);
	m_errorMessage = errorMessage;
}
