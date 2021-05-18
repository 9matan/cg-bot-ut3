using Sharpmake;

[Generate]
public class CodingameUtilityProject : CommonProject
{
    public CodingameUtilityProject()
    {
        Name = "CodingameUtility";
        SourceRootPath = @"[project.SharpmakeCsPath]";
    }
    public override void ConfigureAll(Project.Configuration conf, Target target)
    {
        conf.PrecompHeader = "CodingameUtility_PCH.h";
        conf.PrecompSource = "CodingameUtility_PCH.cpp";

        conf.IncludePaths.Add(@"[project.SourceRootPath]\code");
        conf.IncludePrivatePaths.Add(@"[project.SourceRootPath]");

        conf.Output = Configuration.OutputType.Lib;
    }
}