using System.IO;
using Sharpmake;
using CommonSharpmake;

namespace AOC
{
	[Generate]
	public class AOCProject : CommonProject
	{
		public AOCProject()
		{
			Name = "AdventOfCode";

			RootPath = @"[project.SharpmakeCsPath]\..";
			SourceRootPath = @"[project.RootPath]\src\aoc";
		}

		public override void ConfigureAll(Configuration conf, Target target)
		{
			base.ConfigureAll(conf, target);

			// The common project includes an include for the external path...
			// which includes ALL of foundation again at a higher level. Prevent the double up.
			// $TODO Remove this include from the common project
			conf.IncludePaths.Remove(@"[project.RootPath]\externals");

			conf.AddPublicDependency<FoundationLibAoC>(target);

			conf.Output = Configuration.OutputType.Exe;
		}
	}
}
