using Sharpmake;

[Generate]
public class BotAppProject : CommonProject
{
    public BotAppProject()
    {
        Name = "BotApp";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.AddPrivateDependency<BotCoreProject>(target);
    }
}