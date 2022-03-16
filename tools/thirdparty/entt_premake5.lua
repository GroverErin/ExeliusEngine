local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local entt = {}

function entt.GenerateDependencyProject(dependencyRootFolder)
end

function entt.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "single_include/"
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "/natvis/entt/*.natvis"
        }
    filter {}
end

dependencies.AddDependency("entt", entt)