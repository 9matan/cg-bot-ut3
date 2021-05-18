using Sharpmake;

[module: Sharpmake.Include(@"CodingameUtility\CodingameUtility.sharpmake.cs")]
[module: Sharpmake.Include(@"Sharpmake\Common.sharpmake.cs")]

[Generate]
public class CGBotSolution : CommonSolution
{
	public CGBotSolution()
	{
		Name = "CGBotSolution";
	}
	
    public override void ConfigureAll(Solution.Configuration conf, Target target)
    {
		base.ConfigureAll(conf, target);
		
        conf.SolutionPath = @"[solution.SharpmakeCsPath]\Sharpmake\generated";
		
		conf.AddProject<CodingameUtilityProject>(target);
    }
}

public static class Main
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments arguments)
    {
		KitsRootPaths.SetUseKitsRootForDevEnv(DevEnv.vs2019, KitsRootEnum.KitsRoot10, Options.Vc.General.WindowsTargetPlatformVersion.Latest);

		arguments.Generate<CGBotSolution>();
    }
}