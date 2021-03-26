printf("Including ExeliusBuildDefaults.lua - This should only appear once.")
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
		"Debug",	-- Contains no compiler optimized code and no packaged assets. (Slow and Heavy)
		"Test",     -- Contains compiler optimized code and no packaged assets. (Fast and Heavy)
		"Release"	-- Contains compiler optimized code and packaged assets. (Fast and Light)
	}

	-- Calls a premake command to generate the different platforms.
	platforms
	{
		"Win32",
		"Win64"
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

	targetdir("Build/" .. outputdir)
	objdir("Temp/" .. outputdir)

	filter("platforms:Win64")
		architecture("x86_64")

	filter("platforms:Win32")
		architecture("x86")

	filter("configurations:Debug")
		symbols("full")
		runtime("Debug")
		defines("EXE_DEBUG")
		staticruntime("On")

	filter("configurations:Test")
		optimize("On")
		runtime("Release")
		defines("EXE_TEST")
		staticruntime("On")

	filter("configurations:Release")
		optimize("On")
		runtime("Release")
		defines("EXE_RELEASE")
		staticruntime("On")
		
	-- Reset filters
	filter {}
end

-- Links and includes all dependencies for the Exelius Engine.
function exeliusDefaults.InitializeEngine(dependencies)

	-- Possible Distant Future Dependencies:
	--dependencies.RequireTool("OpenGL")
	--dependencies.RequireTool("Vulkan")

	-- Current Dependencies:
	dependencies.RequireTool("DragonBonesCPP")
	dependencies.RequireTool("EASTL")
	dependencies.RequireTool("imgui")
	dependencies.RequireTool("libsodium")
	dependencies.RequireTool("rapidjson")
	dependencies.RequireTool("SFML")
	dependencies.RequireTool("spdlog")
	dependencies.RequireTool("tmxlite")
	dependencies.RequireTool("zlib")
	--dependencies.RequireTool("TinyXML2")
end

-- Initializes the Exelius Engine in the current project scope.
-- @param engine_root string | "The root path to the engine. Can pass nil for current executing premake file being in the same as the root directory."
function exeliusDefaults.InitializeProject(engine_root)

	engine_root = engine_root or "";

	links("Exelius");

	local incldir = os.realpath(engine_root .. "Exelius/");
	includedirs(incldir);
end

-- Includes the header only libraries into the project.
-- This could probably be better.
function exeliusDefaults.IncludeLibHeaders(dependencies)

	-- Current Header Libraries:
	dependencies.Include("DragonBonesCPP")
	dependencies.Include("EASTL")
	dependencies.Include("imgui")
	dependencies.Include("libsodium")
	dependencies.Include("rapidjson")
	dependencies.Include("SFML")
	dependencies.Include("spdlog")
	dependencies.Include("tmxlite")
	dependencies.Include("zlib")
end

return exeliusDefaults