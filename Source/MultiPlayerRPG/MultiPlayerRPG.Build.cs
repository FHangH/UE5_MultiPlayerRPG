using UnrealBuildTool;

// ReSharper disable once InconsistentNaming
public class MultiPlayerRPG : ModuleRules
{
	public MultiPlayerRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		// ReSharper disable once RedundantExplicitArrayCreation
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"UMG",
			"FhMySQL"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
