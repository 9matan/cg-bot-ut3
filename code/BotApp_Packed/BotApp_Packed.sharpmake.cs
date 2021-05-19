using Sharpmake;

[Generate]
public class BotAppPackedProject : CommonProject
{
    public BotAppPackedProject()
    {
        Name = "BotApp_Packed";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }

    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);

        conf.EventPreBuild.Add($@"[project.SourceRootPath]\{PackedProjectInfo.PackerFileName}");
        conf.Options.Add(Options.Vc.General.DisableFastUpToDateCheck.Enable);

        conf.AddPrivateDependency<BotAppProject>(target, DependencySetting.OnlyBuildOrder);
    }
}