local dependencies = require("PremakeDependancyGenerator")

local rapidjson = {}

function rapidjson.GenerateDependencyProject(dependencyRootFolder)
end

function rapidjson.IncludeDependency(dependencyRootFolder)
end

function rapidjson.LinkDependency(dependencyRootFolder, exeliusLibDir)
end

dependencies.AddDependency("rapidjson", rapidjson)