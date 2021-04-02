printf("Including DragonBonesCPP_premake5.lua - This should only appear once.")
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

local dragonBones = 
{
	location = "DragonBonesCPP"
};

function dragonBones.Include(rootDir)
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
			rootDir .. [[lib/Win32/Release/]]
		}

	filter{"configurations:Release32", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/Release/]]
		}

	filter{"configurations:Debug64", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/Debug/]],
		}

	filter{"configurations:Test64", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/Release/]]
		}

	filter{"configurations:Release64", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/Release/]]
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

function dragonBones.Link(rootdir, exeliusLibDir)

	filter{"system:windows"}
		links
		{
			"dragonBones"
		}

	filter{"system:linux"}
		links
		{
			"dragonBones"
		}

	filter {}
end

dependencies.Add("DragonBonesCPP", dragonBones)