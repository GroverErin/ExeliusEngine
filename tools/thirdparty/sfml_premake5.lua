local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local sfml = {}

function sfml.GenerateDependencyProject(dependencyRootFolder)
    project("sfml-system")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        files
        {
            "%{prj.location}/src/SFML/System/*.hpp",
            "%{prj.location}/src/SFML/System/*.cpp",
        }

        filter {"system:windows"}
            files
            {
                "%{prj.location}/src/SFML/System/Win32/*.hpp",
                "%{prj.location}/src/SFML/System/Win32/*.cpp"
            }

            defines
            {
                "WIN32",
                "_WINDOWS"
            }

        filter {"system:linux"}
            pic("On")

            files
            {
                "%{prj.location}/src/SFML/System/Unix/*.hpp",
                "%{prj.location}/src/SFML/System/Unix/*.cpp"
            }

            defines
            {
                "LINUX"
            }

        filter {}

        includedirs
        {
            "%{prj.location}/include/",
            "%{prj.location}/src/"
        }

        defines
        {
            "SFML_STATIC",
            "_CRT_SECURE_NO_DEPRECATE",
            "_SCL_SECURE_NO_WARNINGS"
        }

    project("sfml-audio")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        files
        {
            "%{prj.location}/src/SFML/Audio/*.*",
            "%{prj.location}/src/SFML/Audio/**.hpp",
        }

        filter {"system:windows"}
            defines
            {
                "WIN32",
                "_WINDOWS"
            }

        filter {"system:linux"}
            pic("On")

            defines
            {
                "LINUX"
            }

        filter {}

        includedirs
        {
            "%{prj.location}/include/",
            "%{prj.location}/src/",
            "%{prj.location}/extlibs/headers/",
            "%{prj.location}/extlibs/headers/AL/"
        }

        defines
        {
            "SFML_STATIC",
            "_CRT_SECURE_NO_DEPRECATE",
            "_SCL_SECURE_NO_WARNINGS",
            "OV_EXCLUDE_STATIC_CALLBACKS",
            "FLAC__NO_DLL"
        }

        links
        {
            "sfml-system"
        }

    project("sfml-window")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        files
        {
            "%{prj.location}/src/SFML/Window/*.cpp",
            "%{prj.location}/src/SFML/Window/*.hpp",
        }

        filter {"system:windows"}
            files
            {
                "%{prj.location}/src/SFML/Window/Win32/**.hpp",
                "%{prj.location}/src/SFML/Window/Win32/**.cpp"
            }

            defines
            {
                "WIN32",
                "_WINDOWS"
            }

        filter {"system:linux"}
            pic("On")

            files
            {
                "%{prj.location}/src/SFML/Window/Unix/**.hpp",
                "%{prj.location}/src/SFML/Window/Unix/**.cpp"
            }

            defines
            {
                "LINUX"
            }

        filter {}

        includedirs
        {
            "%{prj.location}/include/",
            "%{prj.location}/src/",
            "%{prj.location}/extlibs/headers/glad/include/",
            "%{prj.location}/extlibs/headers/vulkan/",
        }

        excludes
        {
            "%{prj.location}/src/SFML/Window/EGLCheck.hpp",
            "%{prj.location}/src/SFML/Window/EGLCheck.cpp",
            "%{prj.location}/src/SFML/Window/EglContext.hpp",
            "%{prj.location}/src/SFML/Window/EglContext.cpp",
        }

        defines
        {
            "SFML_STATIC",
        }

        links
        {
            "sfml-system"
        }

    project("sfml-graphics")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        files
        {
            "%{prj.location}/src/SFML/Graphics/**.cpp",
            "%{prj.location}/src/SFML/Graphics/**.hpp",
        }

        filter {"system:windows"}
            defines
            {
                "WIN32",
                "_WINDOWS"
            }

        filter {"system:linux"}
            pic("On")

            defines
            {
                "LINUX"
            }

        filter {}

        includedirs
        {
            "%{prj.location}/include/",
            "%{prj.location}/src/",
            "%{prj.location}/extlibs/headers/glad/include/",
            "%{prj.location}/extlibs/headers/stb_image/",
            "%{prj.location}/extlibs/headers/freetype2/"
        }

        defines
        {
            "SFML_STATIC",
        }

        links
        {
            "sfml-system",
            "sfml-window"
        }

    project("sfml-network")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        files
        {
            "%{prj.location}/src/SFML/Network/*.cpp",
            "%{prj.location}/src/SFML/Network/*.hpp",
        }

        filter {"system:windows"}
            files
            {
                "%{prj.location}/src/SFML/Network/Win32/**.hpp",
                "%{prj.location}/src/SFML/Network/Win32/**.cpp"
            }

            defines
            {
                "WIN32",
                "_WINDOWS",
                "_WINSOCK_DEPRECATED_NO_WARNINGS"
            }

        filter {"system:linux"}
            pic("On")

            files
            {
                "%{prj.location}/src/SFML/Network/Unix/**.hpp",
                "%{prj.location}/src/SFML/Network/Unix/**.cpp"
            }

            defines
            {
                "LINUX"
            }

        filter {}

        includedirs
        {
            "%{prj.location}/include/",
            "%{prj.location}/src/",
        }

        defines
        {
            "SFML_STATIC",
        }

        links
        {
            "sfml-system"
        }
end

function sfml.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder ..  "include/"
    }

    filter {"system:windows", "architecture:x86"}
        libdirs
        {
            dependencyRootFolder .. "extlibs/libs-msvc/x86/"
        }

    filter {"system:windows", "architecture:x86_64"}
        libdirs
        {
            dependencyRootFolder .. "extlibs/libs-msvc/x64/"
        }

    filter {"system:linux"}
        defines
        {
            "LINUX"
        }

    filter {}

    defines
    {
        "SFML_STATIC",
        "EXELIUS_RENDERER=SFML_RENDERER"
    }
end

function sfml.LinkDependency(dependencyRootFolder, exeliusLibDir)
    filter{"system:windows"}
        links
        {
            "sfml-audio",
            "sfml-graphics",
            "sfml-network",
            "sfml-system",
            "sfml-window",
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

    filter{"system:linux"}
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
end

dependencies.AddDependency("sfml", sfml)