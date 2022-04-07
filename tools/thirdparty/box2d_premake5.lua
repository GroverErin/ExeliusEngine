local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local box2d = {}

function box2d.GenerateDependencyProject(dependencyRootFolder)
    project("box2d")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        warnings("Off")

        files
        {
            "%{prj.location}/src/**.h",
            "%{prj.location}/src/**.cpp"
        }

        includedirs
        {
            "%{prj.location}/include/"
        }
end

function box2d.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder ..  "include/"
    }
end

function box2d.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "box2d"
    }
end

dependencies.AddDependency("box2d", box2d)