local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local zlib = {}

function zlib.GenerateDependencyProject(dependencyRootFolder)
    project("zlib")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        targetdir(exeliusDefaultSettings.BuildOutputDirectory)
        objdir(exeliusDefaultSettings.TempOutputDirectory)

        location(dependencyRootFolder)

        warnings("Off")

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/*.h",
            "%{prj.location}/*.c"
        }
end

function zlib.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder
    }
end

function zlib.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "zlib"
    }
end

dependencies.AddDependency("zlib", zlib)