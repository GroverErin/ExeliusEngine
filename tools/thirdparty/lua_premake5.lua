local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local lua = {}

function lua.GenerateDependencyProject(dependencyRootFolder)
    project("lua")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/*.c",
            "%{prj.location}/*.h"
        }
        removefiles
        {
            "%{prj.location}/onelua.c",
        }

        includedirs
        {
            "%{prj.location}/"
        }
end

function lua.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder,
    }
end

function lua.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "lua"
    }
end

dependencies.AddDependency("lua", lua)