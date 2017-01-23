#include "Michelangelo.h"
#include "CoreMisc.h"
#include "UGameDataSingleton.h"

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
