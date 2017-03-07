// Fill out your copyright notice in the Description page of Project Settings.

#include "Michelangelo.h"
#include "EditorMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "NonUnreal/MichelangeloAPI/CameraParameters.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "Unreal/Web/GrammarSpecificData.h"

using namespace Common;
using namespace MichelangeloAPI;

void UEditorMenu::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	RequestGrammarDataAsync();
}
void UEditorMenu::NativeDestruct()
{
	UUserWidget::NativeDestruct();

	// Clear geometry:
	UGameDataSingleton::Get()->GetRenderItemsCollection()->Clear();
}

void UEditorMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	std::lock_guard<std::mutex> lock(m_eventsQueueMutex);
	while (!m_eventsQueue.empty())
	{
		const auto& internalEvent = m_eventsQueue.front();

		if (internalEvent.Type == InternalEventType::EditorReady)
		{
			std::lock_guard<std::mutex> grammarDataLock(m_grammarDataMutex);
			m_grammarData = UGrammarSpecificData::FromNativeData(internalEvent.GrammarData);
			OnEditorReadyEvent.Broadcast();
		}
		else if (internalEvent.Type == InternalEventType::GrammarEvaluated)
		{
			HandleSceneGeometry(internalEvent.SceneGeometry);
			OnGrammarEvaluatedEvent.Broadcast();
		}
		else if (internalEvent.Type == InternalEventType::GrammarError)
			OnGrammarErrorEvent.Broadcast(Helpers::StringToFString(internalEvent.Message));
		else if (internalEvent.Type == InternalEventType::GrammarWarning)
			OnGrammarWarningEvent.Broadcast(Helpers::StringToFString(internalEvent.Message));

		m_eventsQueue.pop_front();
	}
}

void UEditorMenu::EvaluateGrammar()
{
	auto gameData = UGameDataSingleton::Get();
	auto& nativeWebAPI = gameData->GetWebAPI();

	CameraParameters cameraParameters;
	{
		auto* cameraManager = GEngine->GetFirstLocalPlayerController(gameData->GetSpawner()->GetWorld())->PlayerCameraManager;

		auto position = Helpers::TransformPositionFromUnrealToMichelangeloSpace(cameraManager->GetCameraLocation());
		cameraParameters.SetCameraPosition({ position.X, position.Y, position.Z });

		auto up = Helpers::TransformVectorFromUnrealToMichelangeloSpace(cameraManager->GetActorUpVector());
		cameraParameters.SetCameraUpDirection({ up.X, up.Y, up.Z });

		auto forward = Helpers::TransformVectorFromUnrealToMichelangeloSpace(cameraManager->GetActorForwardVector());
		cameraParameters.SetCameraForwardDirection({ forward.X, forward.Y, forward.Z });

		cameraParameters.SetCameraFov(cameraManager->GetFOVAngle());

		auto viewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		cameraParameters.SetWidth(viewportSize.X);
		cameraParameters.SetHeight(viewportSize.Y);
	}

	// Perform request to evaluate grammar:
	SceneGeometry sceneGeometry;
	std::string message;

	try
	{
		GrammarSpecificData grammarData;
		{
			std::lock_guard<std::mutex> lock(m_grammarDataMutex);
			grammarData = m_grammarData->ToNativeData();
		}
		
		nativeWebAPI.EvaluateGrammar(grammarData, cameraParameters, sceneGeometry, message);
	}
	catch (const std::exception&)
	{
		AddEventToQueue(InternalEvent::OnGrammarErrorEvent("Internal application error."));
		return;
	}

	// If scene is not empty:
	if (!sceneGeometry.IsEmpty())
	{
		// Broadcast that the grammar was evaluated:
		AddEventToQueue(InternalEvent::OnGrammarEvaluatedEvent(sceneGeometry));

		// Output warnings:
		if(!message.empty())
		{
			AddEventToQueue(InternalEvent::OnGrammarWarningEvent(message));
		}
	}

	// If scene is empty:
	else
	{
		// Output errors:
		AddEventToQueue(InternalEvent::OnGrammarErrorEvent(message));
	}	
}
void UEditorMenu::EvaluateGrammarAsync()
{
	auto async = std::bind(&UEditorMenu::EvaluateGrammar, this);
	std::thread(async).detach();
}

void UEditorMenu::RequestGrammarData()
{
	auto& webAPI = UGameDataSingleton::Get()->GetWebAPI();

	std::string grammarID;
	GrammarType grammarType;
	{
		std::lock_guard<std::mutex> lock(m_grammarDataMutex);
		grammarID = Helpers::FStringToString(m_grammarData->ID);
		grammarType = Helpers::UnrealToNativeGrammarType(m_grammarData->GrammarType);
	}

	// Request grammar data:
	auto grammarData = webAPI.GetGrammarData(grammarType, grammarID);

	// Inform that editor is ready:
	AddEventToQueue(InternalEvent::OnEditorReady(grammarData));
}
void UEditorMenu::RequestGrammarDataAsync()
{
	auto async = std::bind(&UEditorMenu::RequestGrammarData, this);
	std::thread(async).detach();
}

void UEditorMenu::SetGrammarData(UGrammarSpecificData* value)
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	m_grammarData = value;
}

FString UEditorMenu::GetName() const
{
	return m_grammarData->Name;
}
FString UEditorMenu::GetType() const
{
	return m_grammarData->Type;
}
FString UEditorMenu::GetCode() const
{
	return m_grammarData->Code;
}
void UEditorMenu::SetCode(const FString& value)
{
	std::lock_guard<std::mutex> lock(m_grammarDataMutex);
	m_grammarData->Code = value;
}

void UEditorMenu::AddEventToQueue(InternalEvent&& internalEvent)
{
	std::lock_guard<std::mutex> lock(m_eventsQueueMutex);
	m_eventsQueue.emplace_back(internalEvent);
}
void UEditorMenu::HandleSceneGeometry(const SceneGeometry& sceneGeometry)
{
	// Spawn actors from objects:
	auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();
	renderItems->Clear();
	renderItems->AddGeometry(sceneGeometry);
}
