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
}
void UEditorMenu::NativeDestruct()
{
	// Clear geometry:
	UGameDataSingleton::Get()->GetRenderItemsCollection()->Clear();
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
		nativeWebAPI.EvaluateGrammar(GrammarData->ToNativeData(), cameraParameters, sceneGeometry, message);
	}
	catch (const std::exception&)
	{
		AddEventToQueue(InternalEvent::OnGrammarErrorEvent("Internal application error."));
		return;
	}

	// If scene is not empty:
	if (!sceneGeometry.IsEmpty())
	{
		// Spawn actors from objects:
		auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();
		renderItems->Clear();
		renderItems->AddGeometry(sceneGeometry);

		// Broadcast that the grammar was evaluated:
		AddEventToQueue(InternalEvent::OnGrammarEvaluatedEvent());

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

void UEditorMenu::SetGrammarData(UGrammarSpecificData* value)
{
	GrammarData = value;
}

void UEditorMenu::AddEventToQueue(InternalEvent&& internalEvent)
{
	std::lock_guard<std::mutex> lock(m_eventsQueueMutex);
	m_eventsQueue.emplace_back(internalEvent);
}
