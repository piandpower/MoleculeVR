// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using System;
using UnrealBuildTool;

public class MoleculeVR : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public MoleculeVR(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});

        PublicDependencyModuleNames.AddRange(new string[] { "ShaderCore", "RenderCore", "RHI",  "RuntimeMeshComponent" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Third Party Libraries

        PublicDelayLoadDLLs.Add("assimp.dll");
        
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp", "Libraries", "assimp.lib"));

        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "assimp", "Includes"));

        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "dirent", "Includes"));
    }
}
