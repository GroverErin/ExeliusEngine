local dependencies = require("PremakeDependancyGenerator")

local imgui = {}

function imgui.GenerateDependencyProject(dependencyRootFolder)
end

function imgui.IncludeDependency(dependencyRootFolder)
end

function imgui.LinkDependency(dependencyRootFolder, exeliusLibDir)
end

dependencies.AddDependency("imgui", imgui)