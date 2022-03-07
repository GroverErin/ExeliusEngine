local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local glfw = {}

function glfw.GenerateDependencyProject(dependencyRootFolder)

end

function glfw.IncludeDependency(dependencyRootFolder)

end

function glfw.LinkDependency(dependencyRootFolder, exeliusLibDir)

end

dependencies.AddDependency("glfw", glfw)