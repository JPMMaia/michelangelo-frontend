#pragma once

#include "Unreal/Mesh/UStaticMeshGenerator.h"
#include "Unreal/Mesh/AInstancedStaticMeshActorManager.h"
#include "UGameDataSingleton.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UGameDataSingleton : public UObject
{
	GENERATED_BODY()
public:
	explicit UGameDataSingleton(const FObjectInitializer& ObjectInitializer);

	UStaticMeshGenerator* GetStaticMeshGenerator();
	AInstancedStaticMeshActorManager* GetInstancedStaticMeshActorManager(UWorld* world);

	UFUNCTION(BlueprintCallable, Category = "Game Data Singleton")
	const FString& GetSavedEmail() const;

	UFUNCTION(BlueprintCallable, Category="Game Data Singleton")
	void SetSavedEmail(const FString& savedEmail);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data Singleton")
	UStaticMeshGenerator* StaticMeshGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data Singleton")
	AInstancedStaticMeshActorManager* InstancedStaticMeshActorManager;

private:
	FString m_savedEmail;

	static constexpr TCHAR* s_loginCredentialsFilename = L"LoginCredentials.data";
};