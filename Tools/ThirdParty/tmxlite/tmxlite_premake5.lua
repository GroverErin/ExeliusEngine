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

	filter{"platforms:x86", "configurations:Debug", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/Debug/]],
		}

	filter{"platforms:x86", "configurations:Test", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/RelWithDebInfo/]]
		}

	filter{"platforms:x86", "configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/Release/]]
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
			rootDir .. [[lib/Win64/Release/]]
		}
		
	--------------------------------------------------------------------------------------------------
	-- Linux Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"platforms:x86", "configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/Debug/]],
		}

	filter{"platforms:x86", "configurations:Test", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin32/RelWithDebInfo/]]
		}

	filter{"platforms:x86", "configurations:Release", "system:linux"}
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
			rootDir .. [[lib/Lin64/RelWithDebInfo/]]
		}

	filter{"platforms:x64", "configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Lin64/Release/]]
		}

	--------------------------------------------------------------------------------------------------
	-- Raspberry Pi Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"platforms:rpi86", "configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi32/Debug/]],
		}

	filter{"platforms:rpi86", "configurations:Test", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi32/RelWithDebInfo/]]
		}

	filter{"platforms:rpi86", "configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi32/Release/]]
		}

	filter{"platforms:rpi64", "configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi64/Debug/]],
		}

	filter{"platforms:rpi64", "configurations:Test", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi64/RelWithDebInfo/]]
		}

	filter{"platforms:rpi64", "configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi64/Release/]]
		}

	filter{}
end

function tmxlite.Link(rootdir, exeliusLibDir)

	filter{"configurations:Debug"}
		links
		{
			"tmxlite-s-d"
		}

	filter{"configurations:Test"}
		links
		{
			"tmxlite"
		}
	
	filter{"configurations:Release"}
		links
		{
			"tmxlite-s"
		}

	filter {}
end

dependencies.Add("tmxlite", tmxlite)