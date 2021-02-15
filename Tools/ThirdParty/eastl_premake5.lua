printf("Including eastl_premake5.lua - This should only appear once.")
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

local eastl = 
{
	location = "EASTL"
};

function eastl.Include(rootDir)
	includedirs
	{
		rootDir .. [[include/]]
	}

	--------------------------------------------------------------------------------------------------
	-- Windows Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"configurations:Debug", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Windows/Debug/]],
		}

	filter{"configurations:Test", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Windows/RelWithDebInfo/]]
		}

	filter{"configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Windows/MinSizeRel/]]
		}

	filter {"architecture:x64", "system:windows"}
		libdirs
		{
			rootDir .. [[extlibs/libs-msvc/x64/]],
		}

	--------------------------------------------------------------------------------------------------
	-- Linux Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Linux/Debug/]],
		}

	filter{"configurations:Test", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Linux/Release/]]
		}

	filter{"configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Linux/Release/]]
		}

	filter{}
end

function eastl.Link(rootdir, exeliusLibDir)

	filter{"configurations:Debug", "system:windows"}
		links
		{
			"EASTL"
		}

	filter{"configurations:Test", "system:windows"}
		links 
		{
			"EASTL"
		}

	filter{"configurations:Release", "system:windows"}
		links 
		{
			"EASTL"
		}

	filter{"configurations:Debug", "system:linux"}
		links
		{
			"EASTL"
		}

	filter{"configurations:Test", "system:linux"}
		links 
		{
			"EASTL"
		}

	filter{"configurations:Release", "system:linux"}
		links 
		{
			"EASTL"
		}

	filter {}
end

dependencies.Add("EASTL", eastl)