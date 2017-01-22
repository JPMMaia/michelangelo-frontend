#include "Michelangelo.h"
#include "CoreMisc.h"
#include "UGameDataSingleton.h"

UGameDataSingleton::UGameDataSingleton(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	StaticMeshGenerator(nullptr),
	InstancedStaticMeshActorManager(nullptr)
{
	TArray<FString> data;
	if(FFileHelper::LoadANSITextFileToStrings(s_loginCredentialsFilename, nullptr, data))
	{
		m_savedEmail = data[0];
	}
	else
	{
		m_savedEmail.Empty();
	}
}

UStaticMeshGenerator* UGameDataSingleton::GetStaticMeshGenerator()
{
	if (!this->StaticMeshGenerator)
		this->StaticMeshGenerator = NewObject<UStaticMeshGenerator>();

	return this->StaticMeshGenerator;
}

AInstancedStaticMeshActorManager* UGameDataSingleton::GetInstancedStaticMeshActorManager(UWorld* world)
{
	if (!this->InstancedStaticMeshActorManager)
		this->InstancedStaticMeshActorManager = world->SpawnActor<AInstancedStaticMeshActorManager>(AInstancedStaticMeshActorManager::StaticClass());

	return this->InstancedStaticMeshActorManager;
}

const FString& UGameDataSingleton::GetSavedEmail() const
{
	return m_savedEmail;
}
void UGameDataSingleton::SetSavedEmail(const FString& savedEmail)
{
	m_savedEmail = savedEmail;
	FFileHelper::SaveStringToFile(m_savedEmail, s_loginCredentialsFilename);
}
