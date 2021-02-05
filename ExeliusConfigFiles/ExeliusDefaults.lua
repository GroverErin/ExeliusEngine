printf("Including ExeliusDefaults.lua - This should only appear once.")
------------------------------------------------------------------------------------------------------
--[[
	Exelius Game Engine - Game Development Tool
	Copyright (c) 2020 Exelius Entertainment
	Created By: Erin "Ryukana" Grover

	This Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	License for more details. You should have received a copy of the
	License along with this Engine.
--]]
------------------------------------------------------------------------------------------------------

-- Exelius Default Settings Script:
-- It sets the workspace and project settings for the Exelius.
-- *From Dylan Wijnen's _utils lua script: https://github.com/dylanwijnen1/DragonEngine/blob/master/tools/modules/dragoncore/_utils.lua

-- Create a new module for premake and get it.
premake.modules.exeliusDefaultSettings = {}
local exeliusDefaults = premake.modules.exeliusDefaultSettings

-- The directory that the Engine AND Projects will output to.
function exeliusDefaults.DefaultOutputDirectory()
	return "%{cfg.buildcfg}_%{cfg.architecture}/%{prj.name}"
end

-- Adds the default settings for Exelius workspaces.
function exeliusDefaults.WorkspaceDefaults()

	-- Calls a premake command to generate the different build configurations.
	configurations
	{
		"Debug",        -- Contains no compiler optimized code and no packaged assets. (Slow and Heavy)
		"Release",      -- Contains compiler optimized code and no packaged assets. (Fast and Heavy)
		"Distribution"  -- Contains compiler optimized code and packaged assets. (Fast and Light)
	}

	-- Calls a premake command to generate the different platforms.
	-- Win32 seems to be depreciated on most libraries and some platforms. SFML doesn't have build options for it. Might be able to be added later.
	platforms
	{
		"x64",
		--"Win32"
	}

	-- Set the compilation process to be multithreaded, this only effects MSVC... not sure how to speed up Linux.
	flags
	{
		"MultiProcessorCompile"
	}
end

-- Adds the default settings for any Exelius project.
-- To override the defaults call this function first then apply your overrides.
function exeliusDefaults.ProjectDefaults()

	-- Gets the output directory of the project.
	local outputdir = exeliusDefaults.DefaultOutputDirectory()

	-- Exelius is written in C++ 17
	language("C++")
	cppdialect("C++17")
	systemversion("latest")

	warnings("Extra")

	staticruntime("On")

	targetdir("Builds/" .. outputdir)
	objdir("Temp/" .. outputdir)

	filter("platforms:x64")
		architecture("x64")

	-- Win32 is being depreciated in most libraries.
	--filter("platforms:Win32")
	--	architecture("x86")

	filter("configurations:Debug")
		symbols("full")
		runtime("Debug")
		defines("EXE_DEBUG")

	filter("configurations:Release")
		optimize("On")
		runtime("Release")
		defines("EXE_RELEASE")

	filter("configurations:Distribution")
		optimize("On")
		runtime("Release")
		defines("EXE_DIST")
		
	-- Reset filters
	filter {}
end

-- Links and includes all dependencies for the Exelius Engine.
function exeliusDefaults.InitializeEngine(dependencies)

	--local dependencies = require("../Dependencies/ExeliusDependencies")	

	-- Planned Dependencies:
	--dependencies.RequireTool("EASTL")
	--dependencies.RequireTool("IMGUI")
	--dependencies.RequireTool("TMXLite")

	-- Possible Distant Future Dependencies:
	--dependencies.RequireTool("OpenGL")
	--dependencies.RequireTool("Vulkan")
	--dependencies.RequireTool("DragonBones")

	-- Current Dependencies:
	dependencies.RequireTool("spdlog") -- Temporary, will write in house Logger.
	dependencies.RequireTool("SFML")
end

-- Initializes the Exelius Engine in the current project scope.
-- @param engine_root string | "The root path to the engine. Can pass nil for current executing premake file being in the same as the root directory."
function exeliusDefaults.InitializeProject(engine_root)

	engine_root = engine_root or "";

	links("Exelius");

	local incldir = os.realpath(engine_root .. "Exelius/ExeliusCore/");
	includedirs(incldir);

	--exeliusDefaults.InitializeEngine();
	--local dependencies = require("../Dependencies/ExeliusDependencies")
	--dependencies.Require("spdlog") -- Temporary, will write in house Logger.
end

return exeliusDefaults