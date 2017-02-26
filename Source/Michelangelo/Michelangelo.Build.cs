// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Michelangelo : ModuleRules
{
    public Michelangelo(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "Json", "JsonUtilities", "ProceduralMeshComponent", "UMG", "PhysX", "APEX" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Add lib curl:
        AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
        PublicIncludePaths.Add("C:/Program Files/Epic Games/UE_4.15/Engine/Source/ThirdParty/libcurl/curl-7.47.1/include/Win64/VS2015");
        PublicAdditionalLibraries.Add("C:/Program Files/Epic Games/UE_4.15/Engine/Source/ThirdParty/libcurl/curl-7.47.1/lib/Win64/VS2015/libcurl_a.lib");
        Definitions.Add("CURL_STATICLIB=1");

        PublicIncludePaths.AddRange(
          new string[] {
                "ThirdParty/boost/spirit/include"
          }
          );
        PrivateIncludePaths.AddRange(
            new string[] {
                "MichelangeloAPI/Source"
            }
            );

        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // Build fast:
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        // Enable exceptions:
        UEBuildConfiguration.bForceEnableExceptions = true;
    }
}
