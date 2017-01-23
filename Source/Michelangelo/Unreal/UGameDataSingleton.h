#pragma once

#include "Unreal/Mesh/UStaticMeshLoader.h"
#include "Unreal/Materials/UMaterialLoader.h"
#include "UGameDataSingleton.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UGameDataSingleton : public UObject
{
	GENERATED_BODY()

public:
	explicit UGameDataSingleton(const FObjectInitializer& ObjectInitializer);

	UStaticMeshLoader* GetStaticMeshLoader();
	UMaterialLoader* GetMaterialLoader();
	
	AActor* GetSpawner() const;
	void SetSpawner(AActor* spawner);

	UFUNCTION(BlueprintCallable, Category = "Game Data Singleton")
	const FString& GetSavedEmail() const;

	UFUNCTION(BlueprintCallable, Category="Game Data Singleton")
	void SetSavedEmail(const FString& savedEmail);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data Singleton")
	UStaticMeshLoader* StaticMeshGenerator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Data Singleton")
	UMaterialLoader* MaterialManager = nullptr;

	UPROPERTY()
	AActor* Spawner = nullptr;

private:
	FString m_savedEmail;

	static constexpr TCHAR* s_loginCredentialsFilename = L"LoginCredentials.data";
};