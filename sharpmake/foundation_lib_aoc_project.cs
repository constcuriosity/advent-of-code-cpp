using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sharpmake;
using Foundation;

namespace AOC
{
	[Generate]
	class FoundationLibAoC : FoundationLibProject
	{
		public FoundationLibAoC()
		{
			// Make the RootPath be the foundation root folder
			RootPath = @"[project.SharpmakeCsPath]\..\externals\foundation";

			// But route to the temp path for this project
			TempPath = @"[project.SharpmakeCsPath]\..\temp";
		}
	}
}
