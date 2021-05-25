using Sharpmake;

[Generate]
public class MimaxCommonProject : CommonProject
{
    public MimaxCommonProject()
    {
        Name = "MimaxCommon";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.PrecompHeader = "MimaxCommon_PCH.h";
        conf.PrecompSource = "MimaxCommon_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.Output = Configuration.OutputType.Lib;
    }
}