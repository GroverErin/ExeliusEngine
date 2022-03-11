local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local glm = {}

function glm.GenerateDependencyProject(dependencyRootFolder)

end

function glm.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "/"
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "util/glm.natvis"
        }
    filter {}
end

function glm.LinkDependency(dependencyRootFolder, exeliusLibDir)

end

dependencies.AddDependency("glm", glm)