#include "Michelangelo.h"
#include "CoreMisc.h"
#include "UGameDataSingleton.h"
#include "UGameDataSingletonLibrary.h"
#include "Unreal/Web/UWebAPI.h"

UGameDataSingleton* UGameDataSingleton::Get()
{
	return UGameDataSingletonLibrary::GetGameDataSingleton();
}

UGameDataSingleton::UGameDataSingleton(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
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

UStaticMeshLoader* UGameDataSingleton::GetStaticMeshLoader()
{
	if (!this->StaticMeshGenerator)
		this->StaticMeshGenerator = NewObject<UStaticMeshLoader>();

	return this->StaticMeshGenerator;
}
UMaterialLoader* UGameDataSingleton::GetMaterialLoader()
{
	if (!this->MaterialManager)
		this->MaterialManager = NewObject<UMaterialLoader>();

	return this->MaterialManager;
}
UWebAPI * UGameDataSingleton::GetWebAPI()
{
	if (!this->WebAPI)
		this->WebAPI = NewObject<UWebAPI>();

	return this->WebAPI;
}
URenderItemsCollection* UGameDataSingleton::GetRenderItemsCollection()
{
	if (!this->RenderItemsCollection)
		this->RenderItemsCollection = NewObject<URenderItemsCollection>();

	return this->RenderItemsCollection;
}
AActor* UGameDataSingleton::GetSpawner() const
{
	return Spawner;
}
void UGameDataSingleton::SetSpawner(AActor* spawner)
{
	Spawner = spawner;
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
