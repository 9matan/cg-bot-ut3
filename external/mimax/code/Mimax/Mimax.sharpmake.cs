using Sharpmake;

[Generate]
public class MimaxProject : CommonProject
{
    public MimaxProject()
    {
        Name = "Mimax";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.PrecompHeader = "Mimax_PCH.h";
        conf.PrecompSource = "Mimax_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.Output = Configuration.OutputType.Lib;
    }
}