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

    File Last Edited: 10/21/2020
    Last Edit Details: Initial Creation
--]]
------------------------------------------------------------------------------------------------------

local dependencies = require("../Dependencies/ExeliusDependencies")

local exeliusDefaults = require("../ExeliusConfigFiles/ExeliusDefaults")

local sfml = 
{
	location = "SFML"
};

function sfml.Project(rootDir)
	-- sfml-graphics
	-- sfml-audio
	-- sfml-system
	-- sfml-window
	printf("Generating SFML Projects.")
	------------------------------------------------------------------------------------------------------
	-- SFML System
	------------------------------------------------------------------------------------------------------
	project "sfml-system"
		location (rootDir .. "Build/")
		kind "StaticLib"

		exeliusDefaults.ProjectDefaults()

		warnings("Off")

		defines
		{
			"SFML_STATIC"
		}

		-- WINDOWS --
		filter("system:windows")
			defines
			{
				"WIN32",
				"_WIN32",
				"_WINDOWS",
				"_CRT_SECURE_NO_DEPRECATE",
				"_SCL_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_WARNINGS"
			}

		filter("system:windows")
			files
			{
				rootDir .. [[src/SFML/System/Win32/*.cpp]],
				rootDir .. [[src/SFML/System/Win32/*.hpp]]
			}

		-- END WINDOWS --

		-- MAC OS --
		filter "system:macosx"
			defines
			{
				"MACOSX"
			}

			--TODO: Make changes if necessary to fix MAC OS support.

		-- END MAC OS --

		-- LINUX --
		filter "system:linux"
			defines
			{
				"LINUX"
			}

			--TODO: Make changes if necessary to fix Linux support.

		-- END LINUX --

		-- Reset filter selector so projects can add to global scope of project. (Link 2)
		filter {}

		files
		{
			rootDir .. [[src/SFML/System/*.hpp]],
			rootDir .. [[src/SFML/System/*.cpp]]
		}

		includedirs
		{
			rootDir .. [[include/]],
			rootDir .. [[src/]]
		}

	------------------------------------------------------------------------------------------------------
	-- SFML Audio
	------------------------------------------------------------------------------------------------------
	project "sfml-audio"
		location (rootDir .. "Build/")
		kind "StaticLib"

		exeliusDefaults.ProjectDefaults()

		warnings("Off")

		defines
		{
			"SFML_STATIC",
			"OV_EXCLUDE_STATIC_CALLBACKS",
			"FLAC__NO_DLL"
		}

		-- WINDOWS --
		filter("system:windows")
			defines
			{
				"WIN32",
				"_WIN32",
				"_WINDOWS",
				"_CRT_SECURE_NO_DEPRECATE",
				"_SCL_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_WARNINGS"
			}

		-- END WINDOWS --

		-- MAC OS --
		filter "system:macosx"
			defines
			{
				"MACOSX"
			}

			--TODO: Make changes if necessary to fix MAC OS support.

		-- END MAC OS --

		-- LINUX --
		filter "system:linux"
			defines
			{
				"LINUX"
			}

			--TODO: Make changes if necessary to fix Linux support.

		-- END LINUX --

		-- Reset filter selector so projects can add to global scope of project. (Link 2)
		filter {}

		files
		{
			rootDir .. [[src/SFML/Audio/*.*]],
			rootDir .. [[src/SFML/Audio/**.hpp]]
		}

		includedirs
		{
			rootDir .. [[include/]],
			rootDir .. [[src/]],
			rootDir .. [[extlibs/headers/]],
			rootDir .. [[extlibs/headers/AL/]]
		}

		links
		{
			"sfml-system"
		}

	------------------------------------------------------------------------------------------------------
	-- SFML Window
	------------------------------------------------------------------------------------------------------
	project "sfml-window"
		location (rootDir .. "Build/")
		kind "StaticLib"

		exeliusDefaults.ProjectDefaults()

		warnings("Off")

		defines
		{
			"SFML_STATIC"
		}

		-- WINDOWS --
		filter("system:windows")
			defines
			{
				"WIN32",
				"_WIN32",
				"_WINDOWS",
				"_CRT_SECURE_NO_DEPRECATE",
				"_SCL_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_WARNINGS"
			}

		filter("system:windows")
			files
			{
				rootDir .. [[src/SFML/Window/Win32/**.cpp]],
				rootDir .. [[src/SFML/Window/Win32/**.hpp]]
			}

		-- END WINDOWS --

		-- MAC OS --
		filter "system:macosx"
			defines
			{
				"MACOSX"
			}

			--TODO: Make changes if necessary to fix MAC OS support.

		-- END MAC OS --

		-- LINUX --
		filter "system:linux"
			defines
			{
				"LINUX"
			}

			--TODO: Make changes if necessary to fix Linux support.

		-- END LINUX --

		-- Reset filter selector so projects can add to global scope of project. (Link 2)
		filter {}

		files
		{
			rootDir .. [[src/SFML/Window/*.hpp]],
			rootDir .. [[src/SFML/Window/*.cpp]]
		}

		excludes
		{
			rootDir .. "src/SFML/Window/EGLCheck.cpp",
			rootDir .. "src/SFML/Window/EGLCheck.hpp",
			rootDir .. "src/SFML/Window/EglContext.cpp",
			rootDir .. "src/SFML/Window/EglContext.hpp"
		}

		includedirs
		{
			rootDir .. [[include/]],
			rootDir .. [[src/]],
			rootDir .. [[extlibs/headers/glad/include/]],
			rootDir .. [[extlibs/headers/vulkan/]]
		}

		links
		{
			"sfml-system"
		}

	------------------------------------------------------------------------------------------------------
	-- SFML Graphics
	------------------------------------------------------------------------------------------------------
	project "sfml-graphics"
		location (rootDir .. "Build/")
		kind "StaticLib"

		exeliusDefaults.ProjectDefaults()

		warnings("Off")

		defines
		{
			"SFML_STATIC"
		}

		-- WINDOWS --
		filter("system:windows")
			defines
			{
				"WIN32",
				"_WIN32",
				"_WINDOWS",
				"_CRT_SECURE_NO_DEPRECATE",
				"_SCL_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_WARNINGS"
			}
		-- END WINDOWS --

		-- MAC OS --
		filter "system:macosx"
			defines
			{
				"MACOSX"
			}

			--TODO: Make changes if necessary to fix MAC OS support.

		-- END MAC OS --

		-- LINUX --
		filter "system:linux"
			defines
			{
				"LINUX"
			}

			--TODO: Make changes if necessary to fix Linux support.

		-- END LINUX --

		-- Reset filter selector so projects can add to global scope of project. (Link 2)
		filter {}

		files
		{
			rootDir .. [[src/SFML/Graphics/**.cpp]],
			rootDir .. [[src/SFML/Graphics/**.hpp]]
		}

		includedirs
		{
			rootDir .. [[include/]],
			rootDir .. [[src/]],
			rootDir .. [[extlibs/headers/stb_image/]],
			rootDir .. [[extlibs/headers/freetype2/]],
			rootDir .. [[extlibs/headers/glad/include/]]
		}

		links
		{
			"sfml-window",
			"sfml-system"
		}

	------------------------------------------------------------------------------------------------------
	-- SFML Network
	------------------------------------------------------------------------------------------------------
	project "sfml-network"
		location (rootDir .. "Build/")
		kind "StaticLib"

		exeliusDefaults.ProjectDefaults()

		warnings("Off")

		defines
		{
			"SFML_STATIC"
		}

		-- WINDOWS --
		filter("system:windows")
			defines
			{
				"WIN32",
				"_WIN32",
				"_WINDOWS",
				"_CRT_SECURE_NO_DEPRECATE",
				"_SCL_SECURE_NO_WARNINGS",
				"_CRT_SECURE_NO_WARNINGS",
				"_WINSOCK_DEPRECATED_NO_WARNINGS"
			}

		filter("system:windows")
			files
			{
				rootDir .. [[src/SFML/Network/Win32/**.cpp]],
				rootDir .. [[src/SFML/Network/Win32/**.hpp]]
			}

		-- END WINDOWS --

		-- MAC OS --
		filter "system:macosx"
			defines
			{
				"MACOSX"
			}

			--TODO: Make changes if necessary to fix MAC OS support.

		-- END MAC OS --

		-- LINUX --
		filter "system:linux"
			defines
			{
				"LINUX"
			}

			--TODO: Make changes if necessary to fix Linux support.

		-- END LINUX --

		-- Reset filter selector so projects can add to global scope of project. (Link 2)
		filter {}

		files
		{
			rootDir .. [[src/SFML/Network/*.cpp]],
			rootDir .. [[src/SFML/Network/*.hpp]]
		}

		includedirs
		{
			rootDir .. [[include/]],
			rootDir .. [[src/]]
		}

		links
		{
			"sfml-system"
		}

	filter {}
end

function sfml.Include(rootDir)
	includedirs
	{
		rootDir .. [[include/]],
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
	links 
	{
		"sfml-graphics",
		"sfml-audio",
		"sfml-system",
		"sfml-window",
		"sfml-network",
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

	libdirs 
	{
		exeliusLibDir .. "sfml-graphics",
		exeliusLibDir .. "sfml-audio",
		exeliusLibDir .. "sfml-system",
		exeliusLibDir .. "sfml-window",
		exeliusLibDir .. "sfml-network",
	}
end

dependencies.Add("SFML", sfml)