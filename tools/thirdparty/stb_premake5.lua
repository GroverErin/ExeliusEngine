local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local stb = {}

function stb.GenerateDependencyProject(dependencyRootFolder)
end

function stb.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "/"
    }

    defines
    {
        "STB_IMAGE_IMPLEMENTATION"
    }
end

dependencies.AddDependency("stb", stb)