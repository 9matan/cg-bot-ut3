using Sharpmake;

[Generate]
public class BotCoreProject : CommonProject
{
    public BotCoreProject()
    {
        Name = "BotCore";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.PrecompHeader = "BotCore_PCH.h";
        conf.PrecompSource = "BotCore_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]\code");
        conf.IncludePaths.Add(@"[project.SourceRootPath]");

        conf.Output = Configuration.OutputType.Lib;
    }
}