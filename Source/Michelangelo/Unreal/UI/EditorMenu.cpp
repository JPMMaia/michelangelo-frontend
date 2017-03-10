#include "Michelangelo.h"
#include "EditorMenu.h"
#include "Unreal/UGameDataSingleton.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "NonUnreal/MichelangeloAPI/CameraParameters.h"
#include "Unreal/Common/UnrealHelpers.h"
#include "Unreal/Web/GrammarSpecificData.h"

using namespace Common;
using namespace MichelangeloAPI;

namespace Events
{
	class OnEditorReadyEvent : public Common::Event<UEditorMenu>
	{
	public:
		explicit OnEditorReadyEvent(const GrammarSpecificData& grammarData) :
			m_grammarData(grammarData)
		{
		}

		void Handle(UEditorMenu& sender) override
		{
			sender.SetGrammarData(UGrammarSpecificData::FromNativeData(m_grammarData));
			sender.OnEditorReadyEvent.Broadcast();
		}

	private:
		GrammarSpecificData m_grammarData;
	};

	class OnGrammarEvaluatedEvent : public Common::Event<UEditorMenu>
	{
	public:
		explicit OnGrammarEvaluatedEvent(const SceneGeometry& sceneGeometry) :
			m_sceneGeometry(sceneGeometry)
		{
		}

		void Handle(UEditorMenu& sender) override
		{
			sender.HandleSceneGeometry(m_sceneGeometry);
			sender.OnGrammarEvaluatedEvent.Broadcast();
		}

	private:
		SceneGeometry m_sceneGeometry;
	};

	class OnGrammarErrorEvent : public Common::Event<UEditorMenu>
	{
	public:
		explicit OnGrammarErrorEvent(const std::string& message) :
			m_message(message)
		{
		}

		void Handle(UEditorMenu& sender) override
		{
			sender.OnGrammarErrorEvent.Broadcast(Helpers::StringToFString(m_message));
		}

	private:
		std::string m_message;
	};

	class OnGrammarWarningEvent : public Common::Event<UEditorMenu>
	{
	public:
		explicit OnGrammarWarningEvent(const std::string& message) :
			m_message(message)
		{
		}

		void Handle(UEditorMenu& sender) override
		{
			sender.OnGrammarWarningEvent.Broadcast(Helpers::StringToFString(m_message));
		}

	private:
		std::string m_message;
	};
}

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

	m_eventsComponent.HandleEvents(*this);
	m_tasksComponent.Update();
}

void UEditorMenu::RequestEvaluateGrammar()
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
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarErrorEvent>("Internal application error."));
		return;
	}

	// If scene is not empty:
	if (!sceneGeometry.IsEmpty())
	{
		// Broadcast that the grammar was evaluated:
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarEvaluatedEvent>(sceneGeometry));

		// Output warnings:
		if(!message.empty())
		{
			m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarWarningEvent>(message));
		}
	}

	// If scene is empty:
	else
	{
		// Output errors:
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarErrorEvent>(message));
	}	
}
void UEditorMenu::RequestEvaluateGrammarAsync()
{
	auto async = std::bind(&UEditorMenu::RequestEvaluateGrammar, this);
	m_tasksComponent.Add(Task(async));
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
	m_eventsComponent.AddEvent(std::make_unique<Events::OnEditorReadyEvent>(grammarData));
}
void UEditorMenu::RequestGrammarDataAsync()
{
	auto async = std::bind(&UEditorMenu::RequestGrammarData, this);
	m_tasksComponent.Add(Task(async));
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

void UEditorMenu::HandleSceneGeometry(const SceneGeometry& sceneGeometry)
{
	// Spawn actors from objects:
	auto renderItems = UGameDataSingleton::Get()->GetRenderItemsCollection();
	renderItems->Clear();

	try
	{
		renderItems->AddGeometry(sceneGeometry);
	}
	catch(const std::exception&)
	{
		m_eventsComponent.AddEvent(std::make_unique<Events::OnGrammarErrorEvent>("Scene geometry sent by server not supported by this application."));
	}
}
