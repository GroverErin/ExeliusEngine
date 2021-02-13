printf("Including sfml_premake5.lua - This should only appear once.")
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

	filter("system:windows")
		defines 
		{
			"SFML_STATIC",
			"EXELIUS_RENDERER=SFML_RENDERER"
		}

	filter("system:linux")
		defines 
		{
			"LINUX",
			"EXELIUS_RENDERER=SFML_RENDERER"
		}

	filter{}
end

function sfml.Link(rootdir, exeliusLibDir)

	filter{"configurations:Debug", "system:windows"}
		links
		{
			"sfml-audio-s-d",
			"sfml-graphics-s-d",
			"sfml-network-s-d",
			"sfml-system-s-d",
			"sfml-window-s-d",
			"flac",
			"freetype",
			"ogg",
			"openal32",
			"vorbis",
			"vorbisenc",
			"vorbisfile",
			"opengl32",
			"gdi32",
			"winmm"
		}

	filter{"configurations:Test", "system:windows"}
		links 
		{
			"sfml-audio-s",
			"sfml-graphics-s",
			"sfml-network-s",
			"sfml-system-s",
			"sfml-window-s",
			"flac",
			"freetype",
			"ogg",
			"openal32",
			"vorbis",
			"vorbisenc",
			"vorbisfile",
			"opengl32",
			"gdi32",
			"winmm"
		}

	filter{"configurations:Release", "system:windows"}
		links 
		{
			"sfml-audio-s",
			"sfml-graphics-s",
			"sfml-network-s",
			"sfml-system-s",
			"sfml-window-s",
			"flac",
			"freetype",
			"ogg",
			"openal32",
			"vorbis",
			"vorbisenc",
			"vorbisfile",
			"opengl32",
			"gdi32",
			"winmm"
		}

	filter{"configurations:Debug", "system:linux"}
		links
		{
			"sfml-audio-d",
			"sfml-graphics-d",
			"sfml-network-d",
			"sfml-system-d",
			"sfml-window-d"
		}

	filter{"configurations:Test", "system:linux"}
		links 
		{
			"sfml-audio",
			"sfml-graphics",
			"sfml-network",
			"sfml-system",
			"sfml-window"
		}

	filter{"configurations:Release", "system:linux"}
		links 
		{
			"sfml-audio",
			"sfml-graphics",
			"sfml-network",
			"sfml-system",
			"sfml-window"
		}

	filter {}
end

dependencies.Add("SFML", sfml)