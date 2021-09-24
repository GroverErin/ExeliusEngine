local log = require("PremakeConsoleLog")
local defaultSettings = require("PremakeSettings")

-- Create a new module for premake and get it.
premake.modules.exeliusDependencyGeneratorClass = {}
local exeliusDependencyGenerator = premake.modules.exeliusDependencyGeneratorClass

-- Create an empty dependency table.
exeliusDependencyGenerator.dependencies = {}

-- Load all dependencies in the dependency folder using the lua scripts.
function LoadDependencyScripts()
    -- Use a relative path here only because it logs nicer. Totally unnessesary.
    local pathToLog = os.realpath(defaultSettings.DependencyScriptLocation)
    log.Log("[Premake] Checking for Premake Scripts at Path: " .. pathToLog)

    local matches = os.matchfiles(defaultSettings.DependencyScriptLocation .. "/*_premake5.lua")
    for _, match in pairs(matches) do
        log.Log("[Premake] Including Premake File '" .. match .."'.")
        include(match)
    end
end

function GenerateDependencyProjects()
    for _, dependency in pairs(exeliusDependencyGenerator.dependencies) do
        local dependencyRootFolder = defaultSettings.DependencyScriptLocation .. "/" .. dependency.location .. "/"
        log.Log("[Premake] Generating Dependancy Project '" .. dependency.location .. "' at: " .. dependencyRootFolder)

        if dependency.GenerateDependencyProject ~= nil then
            dependency.GenerateDependencyProject(dependencyRootFolder)
        else
            log.Warn("[Premake] Dependency [" .. dependency.location .. "] has no GenerateDependencyProject function defined.")
        end
    end
end

-- Adds a given dependency to the table of dependencies.
function exeliusDependencyGenerator.AddDependency(dependencyIdentifier, dependencyClass)
    if exeliusDependencyGenerator.dependencies[dependencyIdentifier] == nil then
        dependencyClass.location = dependencyIdentifier

        exeliusDependencyGenerator.dependencies[dependencyIdentifier] = dependencyClass
        log.Log("[Premake] Registered dependency [" .. dependencyIdentifier .. "]")
    else
        log.Warn("[Premake] Dependency [" .. dependencyIdentifier .. "] already registered.")
    end
end

function exeliusDependencyGenerator.GenerateDependencies()
    LoadDependencyScripts()

    if next(exeliusDependencyGenerator.dependencies) == nil then
        log.Error("[Premake] Dependencies Not Found.")
        return false
    end

    GenerateDependencyProjects()

    return true
end

function exeliusDependencyGenerator.IncludeDependencies()
    for _, dependency in pairs(exeliusDependencyGenerator.dependencies) do
        local dependencyRootFolder = defaultSettings.DependencyScriptLocation .. "/" .. dependency.location .. "/"
        log.Log("[Premake] Including Dependancy Project '" .. dependency.location .. "' from: " .. dependencyRootFolder)

        if dependency.IncludeDependency ~= nil then
            log.Log("[Premake] Including '" .. dependency.location .. "'.")
            dependency.IncludeDependency(dependencyRootFolder)
        else
            log.Warn("[Premake] Dependency [" .. dependency.location .. "] has no IncludeDependency function defined.")
        end
    end
end

function exeliusDependencyGenerator.LinkDependencies()
    for _, dependency in pairs(exeliusDependencyGenerator.dependencies) do
        local dependencyRootFolder = defaultSettings.DependencyScriptLocation .. "/" .. dependency.location .. "/"
        log.Log("[Premake] Linking Dependancy Project '" .. dependency.location .. "' from: " .. dependencyRootFolder)

        if dependency.LinkDependency ~= nil then
            log.Log("[Premake] Linking '" .. dependency.location .. "'.")
            dependency.LinkDependency(dependencyRootFolder)
        else
            log.Warn("[Premake] Dependency [" .. dependency.location .. "] has no LinkDependency function defined.")
        end
    end
end

return exeliusDependencyGenerator