// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MomentTrigger : ModuleRules
{
	public MomentTrigger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// This module has no Public/Private split (all headers live directly under Source/MomentTrigger,
		// now organized into Core/Combat/Karakuri/Enemy subfolders). Without this, UBT's modern default
		// (BuildSettingsVersion.V2+) doesn't add the module's own base directory to the include search
		// path, so cross-folder includes like "Core/MomentTriggerCharacter.h" fail with C1083.
		bLegacyPublicIncludePaths = true;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","GameplayAbilities", "GameplayTasks", "GameplayTags", "StateTreeModule", "GameplayStateTreeModule", "UMG"  });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
