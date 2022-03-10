local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local glad = {}

function glad.GenerateDependencyProject(dependencyRootFolder)
    project("glad")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        filter {"system:linux"}
            pic("On")
        filter {}

        files
        {
            "%{prj.location}/src/glad.c",
            "%{prj.location}/include/glad/glad.h",
            "%{prj.location}/include/KHR/khrplatform.h"
        }

        includedirs
        {
            "%{prj.location}/include/"
        }
end

function glad.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "include/"
    }
end

function glad.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "glad"
    }
end

dependencies.AddDependency("glad", glad)