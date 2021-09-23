local dependencies = require("PremakeDependancyGenerator")

local zlib = {}

function zlib.GenerateDependencyProject(dependencyRootFolder)
end

function zlib.IncludeDependency(dependencyRootFolder)
end

function zlib.LinkDependency(dependencyRootFolder, exeliusLibDir)
end

dependencies.AddDependency("zlib", zlib)