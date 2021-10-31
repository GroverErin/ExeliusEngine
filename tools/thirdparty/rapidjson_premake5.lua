local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local rapidjson = {}

function rapidjson.GenerateDependencyProject(dependencyRootFolder)
end

function rapidjson.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "include/"
    }

    defines
    {
        "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "contrib/natvis/rapidjson.natvis"
        }
    filter {}
end

dependencies.AddDependency("rapidjson", rapidjson)