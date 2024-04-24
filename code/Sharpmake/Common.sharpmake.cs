using System.IO;
using Sharpmake;

public class PackedProjectInfo
{
    public static string PackerFileName = "packer.bat";
}

[Generate]
public class CommonProject : Project
{
    public CommonProject()
    {
        AddTargets(new Target(Platform.win64, DevEnv.vs2022, Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public virtual void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.ProjectPath = @"[project.SharpmakeCsPath]\..\Sharpmake\generated\[project.Name]";
        conf.TargetPath = @"[project.SharpmakeCsPath]\..\..\Bin\[target.Platform]_[target.Optimization]";

        conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
        conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
        conf.Options.Add(Options.Vc.General.PlatformToolset.v143);
        conf.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_22621_0);

        conf.Defines.Add("MIMAX_MCTS_DEBUG=1");
    }
}

[Generate]
public class CommonSolution : Solution
{
    public CommonSolution()
    {

        AddTargets(new Target(Platform.win64, DevEnv.vs2022, Optimization.Debug | Optimization.Release));
    }

    [Configure]
    public virtual void ConfigureAll(Solution.Configuration conf, Target target)
    {
    }
}