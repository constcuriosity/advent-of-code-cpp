using System.Collections.Generic;
using System.IO;
using System.Runtime;
using System.Runtime.InteropServices;
using Sharpmake;
using CommonSharpmake;

[module: Sharpmake.Include(@".\*.cs")]
[module: Sharpmake.Include(@".\..\externals\foundation\sharpmake\utilities\*.cs")]
[module: Sharpmake.Include(@".\..\externals\foundation\sharpmake\projects\foundation_lib_project.cs")]

namespace AOC
{
	[Generate]
	public class AOCSolution : Solution
	{
		public AOCSolution()
		{
			// The name of the solution
			Name = "AdventOfCode";

			AddTargets(CommonTargets.GenerateTargets());
		}

		[Configure]
		public void ConfigureAll(Configuration conf, Target target)
		{
			conf.SolutionPath = @"[solution.SharpmakeCsPath]\..";

			conf.AddProject<FoundationLibAoC>(target);
			conf.AddProject<AOCProject>(target);
			conf.SetStartupProject<AOCProject>();
		}
	}

	public static class Main
	{
		[Sharpmake.Main]
		public static void SharpmakeMain(Sharpmake.Arguments arguments)
		{
			if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
			{
				KitsRootPaths.SetKitsRoot10ToHighestInstalledVersion();
			}

			arguments.Generate<AOCSolution>();
		}
	}
}
