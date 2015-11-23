// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_CLIPS : ModuleRules
{
    private string ModulePath
    {
        get { return System.IO.Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string ThirdPartyPath
    {
        get { return System.IO.Path.GetFullPath(System.IO.Path.Combine(ModulePath, "../../Plugins/")); }
    }
    public UE_CLIPS(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        LoadClips(Target);
	}
    public bool LoadClips(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "64" : "32";
            string LibrariesPath = System.IO.Path.Combine(ThirdPartyPath, "CLIPS", "Libraries");

            PublicAdditionalLibraries.Add(System.IO.Path.Combine(LibrariesPath, "CLIPSStatic" + PlatformString + ".lib")); 
        }
        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(System.IO.Path.Combine(ThirdPartyPath, "CLIPS", "Source", "Integration"));
        }
        Definitions.Add(string.Format("WITH_CLIPS_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
