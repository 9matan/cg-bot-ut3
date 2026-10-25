using Sharpmake;

[Generate]
public class BotUtilityProject : CommonProject
{
    public BotUtilityProject()
    {
        Name = "BotUtility";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.PrecompHeader = "BotUtility_PCH.h";
        conf.PrecompSource = "BotUtility_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.AddPublicDependency<BotCoreProject>(target);
    }
}