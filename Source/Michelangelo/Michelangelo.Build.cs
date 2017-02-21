// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Michelangelo : ModuleRules
{
	public Michelangelo(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "Json", "JsonUtilities", "ProceduralMeshComponent", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Add lib curl:
        AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
        PublicIncludePaths.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/include/Windows");
        PublicAdditionalLibraries.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/lib/Win64/VS2015/libcurl_a.lib");
        Definitions.Add("CURL_STATICLIB=1");

        // Add boost:
        PublicIncludePaths.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/include/Windows");
        PublicAdditionalLibraries.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/lib/Win64/VS2015/libcurl_a.lib");

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
