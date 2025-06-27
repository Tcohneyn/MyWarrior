// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class MyWarrior : ModuleRules
{
	public MyWarrior(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",//
			"CoreUObject",//
			"Engine",//
			"InputCore",//
			"EnhancedInput",//
			"GameplayTags",//
			"GameplayTasks",//
			"Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
	   {
		   "MyWarrior/Public/Characters",
		   "MyWarrior/Public/Items/Weapons"
	   });

           // Uncomment if you are using Slate UI
           // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

           // Uncomment if you are using online features
           // PrivateDependencyModuleNames.Add("OnlineSubsystem");

           // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
   }
}
