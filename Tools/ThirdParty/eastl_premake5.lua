local dependencies = require("PremakeDependancyGenerator")

local eastl = {}

function eastl.GenerateDependencyProject(dependencyRootFolder)
end

function eastl.IncludeDependency(dependencyRootFolder)
end

function eastl.LinkDependency(dependencyRootFolder, exeliusLibDir)
end

dependencies.AddDependency("eastl", eastl)