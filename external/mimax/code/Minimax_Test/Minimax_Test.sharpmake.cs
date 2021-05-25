using Sharpmake;

[Generate]
public class MinimaxTestProject : CommonProject
{
    public MinimaxTestProject()
    {
        Name = "Minimax_Test";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.AddPrivateDependency<MinimaxProject>(target);
    }
}