printf("Including ExeliusDependencies.lua - This should only appear once.")

-- Create a new module for premake and get it.
premake.modules.exeliusDependencies = {}
local exeliusDepends = premake.modules.exeliusDependencies

-- Get the path of this module file.
--local MODULE_ROOT_PATH = path.getabsolute("")

-- Get the path of the workspace relative to this file.
--local EXELIUS_WORKSPACE_ROOT_PATH = path.getabsolute("../")

-- Get the path of the dependacies folder relative to this file.
local EXELIUS_DEPENDENCY_FOLDER_PATH = path.getabsolute("ThirdParty")

-- Get the path of the Exelius output folder relative to this file.
local EXELIUS_OUTPUT_FOLDER_PATH = path.getabsolute("../Builds/ThirdParty")

-- Create an empty dependency table.
exeliusDepends.dependencies = {}

--- Returns the dependency folder relative to the current executing script.
local function DependencyFolder(dependencyRoot, dependency)
	return dependencyRoot .. "/" .. dependency.location .. "/"
end

-- Adds a dependency project to the dependency injector.
-- @param id string | "The id of the dependency"
-- @param dependency table | "A dependency table. @see dependency-example"
function exeliusDepends.Add(id, dependency)
	if exeliusDepends.dependencies[id] == nil then
		dependency.location = dependency.location or id

		exeliusDepends.dependencies[id] = dependency
		printf("Registered dependency [" .. id .. "]")
	else
		term.setTextColor(term.yellow)
		printf("[Warning] Dependency [" .. id .. "] already registered.")
		term.setTextColor()
	end
end

-- Lists all dependencies that are available.
function exeliusDepends.ListDependancies()

	printf("Registered Dependencies: ")
	for id, dependency in pairs(exeliusDepends.dependencies) do
		printf("\t - " .. id)
	end
end

-- Links the dependency to the project.
function exeliusDepends.LinkDependency(dependency)

	-- Dependency Root Folder
	local depRoot = DependencyFolder(EXELIUS_DEPENDENCY_FOLDER_PATH, dependency)
	
	if dependency.Link ~= nil then
		-- Reset Filters
		filter {}

		dependency.Link(depRoot, EXELIUS_OUTPUT_FOLDER_PATH)
	else
		term.setTextColor(term.yellow)
		printf("[Warning] Didn't link dependency. Check if link is required.")
		term.setTextColor()
	end
end

-- Links a dependency to the project.
-- @param id | "The dependency id the dependency has registered with."
function exeliusDepends.Link(id)

	printf("Linking dependency [" .. id .. "]")

	local dependency = exeliusDepends.dependencies[id]

	if dependency ~= nil then
		exeliusDepends.LinkDependency(dependency)
	else
		term.setTextColor(term.red)
		printf("[Error] Dependency not linked [" .. id .. "]")
		term.setTextColor()
	end
end

-- Includes a dependency into the project.
-- @param dependency table | "A dependency object."
function exeliusDepends.IncludeDependency(dependency)
	
	-- Dependency Root Folder
	local depRoot = DependencyFolder(EXELIUS_DEPENDENCY_FOLDER_PATH, dependency)
		
	if dependency.Include ~= nil then
		-- Reset Filters
		filter {}
		
		dependency.Include(depRoot)
	end
end

-- Setup Include Folders for the dependency project.
-- @param id | "The dependency id the dependency has registered with."
function exeliusDepends.Include(id)

	local dependency = exeliusDepends.dependencies[id]

	if dependency ~= nil then
		exeliusDepends.IncludeDependency(dependency)
	else
		term.setTextColor(term.red)
		printf("[Error] Dependency not included [" .. id .. "]")
		term.setTextColor()
	end
end

-- Links and includes the dependency.
function exeliusDepends.RequireTool(id)

	printf("Requiring dependency [" .. id .. "]")

	local dependency = exeliusDepends.dependencies[id]
	if dependency ~= nil then
		exeliusDepends.IncludeDependency(dependency)
		exeliusDepends.LinkDependency(dependency)
	else
		term.setTextColor(term.red)
		printf("[Error] Dependency not found [" .. id .. "]")
		term.setTextColor()
	end
end

-- Generates the project for the dependency.
function exeliusDepends.GenerateProjects()

	for _, dependency in pairs(exeliusDepends.dependencies) do

		local depRoot = DependencyFolder(EXELIUS_DEPENDENCY_FOLDER_PATH, dependency)

		if dependency.Project ~= nil then
			dependency.Project(depRoot)
		end
	end
end

-- Load all dependencies in the dependency folder using the lua scripts.
function exeliusDepends.Initialize()
	local matches = os.matchfiles(EXELIUS_DEPENDENCY_FOLDER_PATH .. "/*.lua")
	for _, match in pairs(matches) do
		include(match)
	end
end

-- Return the module.
return exeliusDepends