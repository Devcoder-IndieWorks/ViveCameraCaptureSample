// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ViveCameraCaptureSampleTarget : TargetRules
{
	public ViveCameraCaptureSampleTarget( TargetInfo Target ) : base( Target )
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ViveCameraCaptureSample" } );
	}
}
