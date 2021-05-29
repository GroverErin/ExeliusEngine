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
}

function sfml.Include(rootDir)
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
			rootDir .. [[extlibs/libs-msvc/x86/]]
		}

	filter{"platforms:x86", "configurations:Test", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/RelWithDebInfo/]],
			rootDir .. [[extlibs/libs-msvc/x86/]]
		}

	filter{"platforms:x86", "configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win32/MinSizeRel/]],
			rootDir .. [[extlibs/libs-msvc/x86/]]
		}

	filter{"platforms:x64", "configurations:Debug", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/Debug/]],
			rootDir .. [[extlibs/libs-msvc/x64/]]
		}

	filter{"platforms:x64", "configurations:Test", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/RelWithDebInfo/]],
			rootDir .. [[extlibs/libs-msvc/x64/]]
		}

	filter{"platforms:x64", "configurations:Release", "system:windows"}
		libdirs
		{
			rootDir .. [[lib/Win64/MinSizeRel/]],
			rootDir .. [[extlibs/libs-msvc/x64/]]
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
			rootDir .. [[lib/Lin32/Release/]]
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
			rootDir .. [[lib/Lin64/Release/]]
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
			rootDir .. [[lib/Rpi32/Release/]]
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
			rootDir .. [[lib/Rpi64/Release/]]
		}

	filter{"platforms:rpi64", "configurations:Release", "system:linux"}
		libdirs
		{
			rootDir .. [[lib/Rpi64/Release/]]
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
			"sfml-audio-s-d",
			"sfml-graphics-s-d",
			"sfml-network-s-d",
			"sfml-window-s-d",
			"sfml-system-s-d",
			"pthread",
			"X11",
			"dl",
			"Xrandr",
			"Xcursor",
			"udev"
		}

	filter{"configurations:Test", "system:linux"}
		links 
		{
			"sfml-audio",
			"sfml-graphics",
			"sfml-network",
			"sfml-window",
			"sfml-system",
			"pthread",
			"X11",
			"dl",
			"Xrandr",
			"Xcursor",
			"udev"
		}

	filter{"configurations:Release", "system:linux"}
		links 
		{
			"sfml-audio-s",
			"sfml-graphics-s",
			"sfml-network-s",
			"sfml-window-s",
			"sfml-system-s",
			"pthread",
			"X11",
			"dl",
			"Xrandr",
			"Xcursor",
			"udev"
		}

	filter {}
end

dependencies.Add("SFML", sfml)