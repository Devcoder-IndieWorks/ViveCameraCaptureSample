// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ViveCameraCapture : ModuleRules
{
	public ViveCameraCapture( ReadOnlyTargetRules Target ) : base( Target )
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = false;
		
		PrivateIncludePaths.AddRange(
		    new string[] {
				"ViveCameraCapture/Private"
			});
			
		PrivateDependencyModuleNames.AddRange(
		    new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"CinematicCamera",
                "ActorLayerUtilities"
			} );
	}
}