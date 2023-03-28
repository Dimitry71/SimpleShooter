// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestTask : ModuleRules
{
	public TestTask(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","AIModule", "NavigationSystem" ,"EnhancedInput", });
	}
}
