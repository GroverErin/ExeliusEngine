printf("Including zlib_premake5.lua - This should only appear once.")
------------------------------------------------------------------------------------------------------
--[[
    Exelius Game Engine - Game Development Tool
    Copyright (c) 2020 Exelius Entertainment
    Created By: Erin "Ryukana" Grover

    This Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    License for more details. You should have received a copy of the
    License along with this Engine.
--]]
------------------------------------------------------------------------------------------------------

local dependencies = require("Tools/ThirdParty/DependencyManager")

local zlib = 
{
	location = "zlib"
};

function zlib.Include(rootDir)
	includedirs
	{
		rootDir .. [[include/]]
	}

	--------------------------------------------------------------------------------------------------
	-- Windows Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"configurations:Debug32", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/Debug/]],
		}

	filter{"configurations:Test32", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/RelWithDebInfo/]]
		}

	filter{"configurations:Release32", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/MinSizeRel/]]
		}

	filter{"configurations:Debug64", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/Debug/]],
		}

	filter{"configurations:Test64", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/RelWithDebInfo/]]
		}

	filter{"configurations:Release64", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/MinSizeRel/]]
		}

	--------------------------------------------------------------------------------------------------
	-- Linux Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"configurations:Debug32", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Debug/]],
		}

	filter{"configurations:Test32", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Release/]]
		}

	filter{"configurations:Release32", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Release/]]
		}

	filter{"configurations:Debug64", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Debug/]],
		}

	filter{"configurations:Test64", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Release/]]
		}

	filter{"configurations:Release64", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Release/]]
		}

	filter{}
end

function zlib.Link(rootdir, exeliusLibDir)

	filter{"configurations:Debug*", "system:windows"}
		links
		{
			"zlibstaticd",
		}

	filter{"configurations:Test*", "system:windows"}
		links 
		{
			"zlibstatic",
		}

	filter{"configurations:Release*", "system:windows"}
		links 
		{
			"zlibstatic",
		}

	filter{"configurations:Debug*", "system:linux"}
		links
		{
			"zlibstaticd",
		}

	filter{"configurations:Test*", "system:linux"}
		links 
		{
			"zlibstatic",
		}

	filter{"configurations:Release*", "system:linux"}
		links 
		{
			"zlibstatic",
		}

	filter {}
end

dependencies.Add("zlib", zlib)