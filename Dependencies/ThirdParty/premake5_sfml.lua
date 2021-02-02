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

local dependencies = require("../Dependencies/ExeliusDependencies")

local exeliusDefaults = require("../ExeliusConfigFiles/ExeliusDefaults")

local sfml = 
{
	location = "SFML"
};

function sfml.Include(rootDir)
	includedirs
	{
		rootDir .. [[include/]]
	}

	filter("configurations:Debug")
		libdirs
		{
			rootDir .. [[lib/Debug/]]
		}

	filter("configurations:Release")
		libdirs
		{
			rootDir .. [[lib/RelWithDebinfo/]]
		}

	filter("configurations:Distribution")
		libdirs
		{
			rootDir .. [[lib/MinSizeRel/]]
		}

	filter "architecture:x64"
		libdirs
		{
			rootDir .. [[extlibs/libs-msvc/x64/]],
		}

	filter "architecture:x86"
		libdirs
		{
			rootDir .. [[extlibs/libs-msvc/x86/]],
		}
	
	-- Reset Filter
	filter {}

	defines 
	{
		"SFML_STATIC",
		"EXELIUS_RENDERSKIN=EXELIUS_RENDERSKIN_SFML"
	}
end

function sfml.Link(rootdir, exeliusLibDir)

	filter("configurations:Debug")
		links
		{
			"sfml-audio-s-d.lib",
			"sfml-graphics-s-d.lib",
			"sfml-main-d.lib",
			"sfml-network-s-d.lib",
			"sfml-system-s-d.lib",
			"sfml-window-s-d.lib"
		}

	filter("configurations:Release")
		links
		{
			"sfml-audio-s.lib",
			"sfml-graphics-s.lib",
			"sfml-main.lib",
			"sfml-network-s.lib",
			"sfml-system-s.lib",
			"sfml-window-s.lib"
		}

	filter("configurations:Distribution")
		links
		{
			"sfml-audio-s.lib",
			"sfml-graphics-s.lib",
			"sfml-main.lib",
			"sfml-network-s.lib",
			"sfml-system-s.lib",
			"sfml-window-s.lib"
		}

	filter{}
	
	links 
	{
		"flac.lib",
		"freetype.lib",
		"ogg.lib",
		"openal32.lib",
		"vorbis.lib",
		"vorbisenc.lib",
		"vorbisfile.lib",
		"opengl32.lib",
		"gdi32.lib",
		"winmm.lib"
	}
end

dependencies.Add("SFML", sfml)