using Sharpmake;

[Generate]
public class MinimaxProject : CommonProject
{
    public MinimaxProject()
    {
        Name = "Minimax";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.PrecompHeader = "Minimax_PCH.h";
        conf.PrecompSource = "Minimax_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.Output = Configuration.OutputType.Lib;
    }
}