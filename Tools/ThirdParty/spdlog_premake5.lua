local dependencies = require("PremakeDependancyGenerator")

local spdlog = {}

function spdlog.GenerateDependencyProject(dependencyRootFolder)
end

function spdlog.IncludeDependency(dependencyRootFolder)
end

function spdlog.LinkDependency(dependencyRootFolder, exeliusLibDir)
end

dependencies.AddDependency("spdlog", spdlog)