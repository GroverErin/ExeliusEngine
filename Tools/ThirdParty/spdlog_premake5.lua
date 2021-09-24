local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local spdlog = {}

function spdlog.GenerateDependencyProject(dependencyRootFolder)
    project("spdlog")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        targetdir(exeliusDefaultSettings.BuildOutputDirectory)
        objdir(exeliusDefaultSettings.TempOutputDirectory)

        location(dependencyRootFolder)

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/src/*.cpp",
        }

        includedirs
        {
            "%{prj.location}/include/",
        }

        defines
        {
            "SPDLOG_COMPILED_LIB"
        }
end

function spdlog.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "include/"
    }
end

function spdlog.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "spdlog"
    }
end

dependencies.AddDependency("spdlog", spdlog)