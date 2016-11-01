// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MichelangeloPlugin : ModuleRules
{
	public MichelangeloPlugin(TargetInfo Target)
	{
        // Add required public include paths:
        PublicIncludePaths.AddRange(
			new string[] {
				"MichelangeloPlugin/Public",
                "MichelangeloPlugin/Common",
                "MichelangeloPlugin/MichelangeloAPI",
                "MichelangeloPlugin/nlohmann/JSON"
			}
			);

        // Add required private include paths:
        PrivateIncludePaths.AddRange(
			new string[] {
				"MichelangeloPlugin/Private",
                "MichelangeloPlugin/Common",
                "MichelangeloPlugin/MichelangeloAPI"
			}
			);

        // Add public dependencies that you statically link with:
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Http",
                "Json",
                "JsonUtilities"
			}
			);

        // Add private dependencies that you statically link with:
        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "OnlineSubsystem"
			}
			);

        // Add any modules that your module loads dynamically:
        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);

        // Add lib curl:
        AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
        PublicIncludePaths.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/include/Windows");
        PublicAdditionalLibraries.Add("C:/Program Files (x86)/Epic Games/4.13/Engine/Source/ThirdParty/libcurl/lib/Win64/VS2015/libcurl_a.lib");
        Definitions.Add("CURL_STATICLIB=1");

        // Build fast:
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
    }
}
