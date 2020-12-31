-- Dependancy injector tool:
-- It automatically detects all the third party tools/libraries located in the ThirdParty folder,
-- then adds them to the solution.
-- *Heavily copied from Dylan Wijnen's dependancy injector tool: https://github.com/dylanwijnen1/DragonEngine/blob/master/tools/modules/dependency-injector/dependency-injector.lua*

-- Create a new module for premake and get it.
premake.modules.exeliusDependencies = {}
local exeliusDepends = premake.modules.exeliusDependencies

-- Get the path of this module file.
--local MODULE_ROOT_PATH = path.getabsolute("");

-- Get the path of the workspace relative to this file.
--local EXELIUS_WORKSPACE_ROOT_PATH = path.getabsolute("../")

-- Get the path of the dependacies folder relative to this file.
local EXELIUS_DEPENDENCY_FOLDER_PATH = path.getabsolute("ThirdParty")

-- Get the path of the Exelius output folder relative to this file.
local EXELIUS_OUTPUT_FOLDER_PATH = path.getabsolute("../Output/ThirdParty")

-- Create an empty dependency table.
exeliusDepends.dependencies = {}


--- Returns the dependency folder relative to the current executing script.
local function DependencyFolder(dependencyRoot, dependency)
	return dependencyRoot .. "/" .. dependency.location .. "/"
end

-- @ref dependency-example 
-- local dep = 
-- {
--		project		= function(rootdir) end
--		include		= function(rootdir) end
--		link		= function(rootdir, outputdir) end
-- }

-- Adds a dependency project to the dependency injector.
-- @param id string | "The id of the dependency"
-- @param dependency table | "A dependency table. @see dependency-example"
function exeliusDepends.Add(id, dependency)

	if exeliusDepends.dependencies[id] == nil then
		dependency.location = dependency.location or id;

		exeliusDepends.dependencies[id] = dependency
		printf("Registered dependency [" .. id .. "]")
	else
		printf("Dependency with id [" .. id .. "] already registered.")
	end
end

-- Lists all dependencies that are available.
function exeliusDepends.ListDependancies()

	printf("Registered Dependencies: ")
	for id, dependency in pairs(exeliusDepends.dependencies) do
		printf("\t - " .. id);
	end
end

function exeliusDepends.PrintDetails(id)

	for _, dependency in pairs(exeliusDepends.dependencies) do
		if dependency.shortname ~= nil then
			printf("\t" .. dependency.shortname);
		end

		if dependency.description ~= nil then
			printf("\t" .. dependency.description);
		end

		printf("\t" .. dependency.location);
		printf("\n\n");
	end
end

-- Links the dependency to the project.
function exeliusDepends.LinkDependency(dependency)

	-- Dependency Root Folder
	local depRoot = DependencyFolder(EXELIUS_DEPENDENCY_FOLDER_PATH, dependency);
	
	if dependency.link ~= nil then
		-- Reset Filters
		filter {}

		dependency.Link(depRoot, EXELIUS_OUTPUT_FOLDER_PATH);
	end
end

-- Links a dependency to the project.
-- @param id | "The dependency id the dependency has registered with."
function exeliusDepends.Link(id)

	local dependency = exeliusDepends.dependencies[id];

	if dependency ~= nil then
		exeliusDepends.LinkDependency(dependency);
	else
		printf("Could not link the dependency: " .. id .. ".");
	end
end

-- Links all dependencies at once.
function exeliusDepends.LinkAll()

	for _, dependency in pairs(exeliusDepends.dependencies) do
		exeliusDepends.LinkDependency(dependency);
	end
end

-- Includes a dependency into the project.
-- @param dependency table | "A dependency object."
function exeliusDepends.IncludeDependency(dependency)
	
	-- Dependency Root Folder
	local depRoot = DependencyFolder(EXELIUS_DEPENDENCY_FOLDER_PATH, dependency);
		
	if dependency.Include ~= nil then
		-- Reset Filters
		filter {}
		
		dependency.Include(depRoot);
	end
end

-- Setup Include Folders for the dependency project.
-- @param id | "The dependency id the dependency has registered with."
function exeliusDepends.Include(id)

	local dependency = exeliusDepends.dependencies[id];

	if dependency ~= nil then
		exeliusDepends.IncludeDependency(dependency);
	else
		printf("Could not include the dependency: " .. id .. ".");
	end
end

-- Includes all dependencies into the project.
function exeliusDepends.IncludeAll()

	for _, dependency in pairs(exeliusDepends.dependencies) do
		exeliusDepends.IncludeDependency(dependency)
	end
end

-- Links and includes the dependency.
function exeliusDepends.Require(id)

	local dependency = exeliusDepends.dependencies[id];

	if dependency ~= nil then
		exeliusDepends.IncludeDependency(dependency);
		exeliusDepends.LinkDependency(dependency);
	else
		printf("Could not find the dependency: " .. id .. ".");
	end
end

-- Generates the project for the dependency.
function exeliusDepends.GenerateProjects()

	for _, dependency in pairs(exeliusDepends.dependencies) do

		local depRoot = DependencyFolder(EXELIUS_DEPENDENCY_FOLDER_PATH, dependency);

		if dependency.Project ~= nil then
			dependency.Project(depRoot);
		end
	end
end

-- Load all dependencies in the dependency folder.
function exeliusDepends.Initialize()

	local matches = os.matchfiles(EXELIUS_DEPENDENCY_FOLDER_PATH .. "/*.lua");
	for _, match in pairs(matches) do
		include(match);
	end
end

-- Return the module.
return exeliusDepends;