#pragma once

#include "Unreal/Mesh/StaticMeshLoader.h"
#include "Unreal/Materials/UMaterialLoader.h"
#include "Unreal/Scene/URenderItemsCollection.h"
#include "NonUnreal/MichelangeloAPI/NativeWebAPI.h"
#include "UGameDataSingleton.generated.h"

class UWebAPI;

UCLASS(Blueprintable, BlueprintType)
class UGameDataSingleton : public UObject
{
	GENERATED_BODY()

public:
	static UGameDataSingleton* Get();

public:
	explicit UGameDataSingleton(const FObjectInitializer& ObjectInitializer);

	UStaticMeshLoader* GetStaticMeshLoader();
	UMaterialLoader* GetMaterialLoader();
	MichelangeloAPI::NativeWebAPI& GetWebAPI();

	UFUNCTION(BlueprintCallable, Category = "Game Data Singleton")
	URenderItemsCollection* GetRenderItemsCollection();
	
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
	URenderItemsCollection* RenderItemsCollection = nullptr;

	UPROPERTY()
	AActor* Spawner = nullptr;

private:
	FString m_savedEmail;

	MichelangeloAPI::NativeWebAPI m_webAPI;

	static constexpr TCHAR* s_loginCredentialsFilename = L"LoginCredentials.data";
};