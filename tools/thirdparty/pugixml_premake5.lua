local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local pugixml = {}

function pugixml.GenerateDependencyProject(dependencyRootFolder)
end

function pugixml.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "src/"
    }

    defines
    {
        "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
        "PUGIXML_HEADER_ONLY"
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "/scripts/natvis/pugixml.natvis"
        }
    filter {}
end

dependencies.AddDependency("pugixml", pugixml)