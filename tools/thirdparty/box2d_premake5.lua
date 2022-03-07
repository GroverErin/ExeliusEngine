local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local box2d = {}

function box2d.GenerateDependencyProject(dependencyRootFolder)
end

function box2d.IncludeDependency(dependencyRootFolder)
end

function box2d.LinkDependency(dependencyRootFolder, exeliusLibDir)
end

dependencies.AddDependency("box2d", box2d)