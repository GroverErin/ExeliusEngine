printf("Including premake5_sfml.lua - This should only appear once.")
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

local dependencies = require("Dependencies/ExeliusDependencies")

local sfml = 
{
	location = "SFML"
};

function sfml.Include(rootDir)
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
			rootDir .. [[lib/Debug/]],
		}

	filter{"configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/RelWithDebinfo/]]
		}

	filter{"configurations:Distribution", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/MinSizeRel/]]
		}

	filter {"architecture:x64", "system:windows"}
		libdirs
		{
			rootDir .. [[extlibs/libs-msvc/x64/]],
		}

	--filter {"architecture:x86", "system:windows"}
		--libdirs
		--{
		--	rootDir .. [[extlibs/libs-msvc/x86/]],
		--}

	--------------------------------------------------------------------------------------------------
	-- Linux Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"configurations:Debug", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Linux/Debug/]],
		}

	filter{"configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Linux/Release/]]
		}

	filter{"configurations:Distribution", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Linux/Release/]]
		}

	filter("system:windows")
		defines 
		{
			"SFML_STATIC",
			"EXELIUS_RENDERSKIN=EXELIUS_RENDERSKIN_SFML"
		}

	filter("system:linux")
		defines 
		{
			"LINUX",
			"EXELIUS_RENDERSKIN=EXELIUS_RENDERSKIN_SFML"
		}

	filter{}
end

function sfml.Link(rootdir, exeliusLibDir)

	filter("configurations:Debug")
		links
		{
			"sfml-audio-d",
			"sfml-graphics-d",
			"sfml-network-d",
			"sfml-system-d",
			"sfml-window-d"
		}

	filter("configurations:Release")
		links 
		{
			"sfml-audio",
			"sfml-graphics",
			"sfml-network",
			"sfml-system",
			"sfml-window"
		}

	filter("configurations:Distribution")
		links 
		{
			"sfml-audio",
			"sfml-graphics",
			"sfml-network",
			"sfml-system",
			"sfml-window"
			-- "flac", -- May be necessary for windows.
			-- "freetype",
			-- "ogg",
			-- "vorbis",
			-- "vorbisenc",
			-- "vorbisfile",
		}

	filter {}
end

dependencies.Add("SFML", sfml)