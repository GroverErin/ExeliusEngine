local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local sol2 = {}

function sol2.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "include/",
        dependencyRootFolder .. "single/"
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "sol2.natvis"
        }
    filter {}
end

dependencies.AddDependency("sol2", sol2)