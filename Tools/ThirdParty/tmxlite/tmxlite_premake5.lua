printf("Including tmxlite_premake5.lua - This should only appear once.")
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

local tmxlite = 
{
	location = "tmxlite"
}

function tmxlite.Include(rootDir)
	includedirs
	{
		rootDir .. [[include/]]
	}

	--------------------------------------------------------------------------------------------------
	-- Windows Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"platforms:Win32", "configurations:Debug", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/Debug/]],
		}

	filter{"platforms:Win32", "configurations:Test", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/RelWithDebInfo/]]
		}

	filter{"platforms:Win32", "configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/MinSizeRel/]]
		}

	filter{"platforms:x64", "configurations:Debug", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/Debug/]],
		}

	filter{"platforms:x64", "configurations:Test", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/RelWithDebInfo/]]
		}

	filter{"platforms:x64", "configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/MinSizeRel/]]
		}
		
	--------------------------------------------------------------------------------------------------
	-- Linux Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"platforms:Win32", "configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Debug/]],
		}

	filter{"platforms:Win32", "configurations:Test", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Release/]]
		}

	filter{"platforms:Win32", "configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Release/]]
		}

	filter{"platforms:x64", "configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Debug/]],
		}

	filter{"platforms:x64", "configurations:Test", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Release/]]
		}

	filter{"platforms:x64", "configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Release/]]
		}

	filter{}
end

function tmxlite.Link(rootdir, exeliusLibDir)

	filter{"configurations:Debug", "system:windows"}
		links
		{
			"tmxlite-s-d"
		}

	filter{"configurations:Test", "system:windows"}
		links
		{
			"tmxlite"
		}

	filter{"configurations:Release", "system:windows"}
		links
		{
			"tmxlite"
		}

	filter{"system:linux"}
		links
		{
			"tmxlite"
		}

	filter {}
end

dependencies.Add("tmxlite", tmxlite)