// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Michelangelo : ModuleRules
{
	public Michelangelo(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Add lib curl:
        AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
        PublicIncludePaths.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/include/Windows");
        PublicAdditionalLibraries.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/lib/Win64/VS2015/libcurl_a.lib");
        Definitions.Add("CURL_STATICLIB=1");

        // Build fast:
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        PublicIncludePaths.AddRange(
            new string[] {
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/Public",
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/Common",
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/MichelangeloAPI",
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/nlohmann/JSON"
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/Private",
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/Common",
                "Plugins/MichelangeloPlugin/Source/MichelangeloPlugin/MichelangeloAPI"
				// ... add other private include paths required here ...
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
