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

-- Define available arguments for this script.
newoption
{
	trigger = "configuration",
	value = "Config",
	description = "The configuration that Premake should generate.",
	allowed = {
		{"Release", "Release (Lightweight, Fast - Optimized w/o Debug)"},
		{"Test", "Test (Heavy, Fast - Optimized w/ Debug)"},
		{"Debug", "Debug (Heavy, Slow - Unoptimized)"}
	}
}

newoption
{
	trigger = "architecture",
	value = "Arch",
	description = "The architecture that Premake should generate for.",
	allowed = {
		{"rpi64", "Raspberry Pi x64"},
		{"rpi86", "Raspberry Pi x86"},
		{"x64", "Linux x64"},
		{"x86", "Linux x86"}
	}
}

term.setTextColor(term.white)
printf("--------- Beginning Premake File Generation ---------")

-- Main Exelius Solution/Workspace and Project Generation Script.

-- The module containing the default settings for the Premake generation.
-- This should include the architecture settings as well as the
-- build configurations, and similar settings.
local exeliusDefaults = require("ExeliusConfigFiles/ExeliusBuildDefaults")

-- The module containing the dependancies and third party tools used by Exelius.
local exeliusDepends = require("Tools/ThirdParty/DependencyManager")

-- Prepares the modules to be used.
exeliusDepends.Initialize()

--local matches = package.loaded
--for thing, match in pairs(matches) do
--	printf(thing)
--end

------------------------------------------------------------------------------------------------------
-- Exelius Solution/Workspace Creation
------------------------------------------------------------------------------------------------------

printf("----- Creating ExeliusEngine Solution/Workspace -----")

-- Call Premake functions to generate the .sln file or platform/compiler specific files
workspace("ExeliusEngine")
	startproject("EXESandbox") -- Eventually this will be replaced with the Editor.

	-- Retrieve and Set all of the default settings for the workspace.
	exeliusDefaults.WorkspaceDefaults()

------------------------------------------------------------------------------------------------------
-- Exelius Dependancy Creation/Preperation
------------------------------------------------------------------------------------------------------

-- Create all of the Projects for the Third-party tools and other dependancies used by Exelius.
-- Having this allows for the Engine to auto-detect Third-party tools and include them
-- into the Solution/Workspace. This only effects tools that have a .lua script that has a
-- GenerateProjects function.
exeliusDepends.GenerateProjects()

------------------------------------------------------------------------------------------------------
-- Exelius Engine Creation
------------------------------------------------------------------------------------------------------

printf("---------- Creating Exelius Engine Project ----------")

-- Call Premake functions to generate .vcxproj files or platform/compiler specific files.
project("Exelius")

	-- Retrieve and Set all of the default settings for the project.
	exeliusDefaults.ProjectDefaults()

	-- Premake function creates the project directory with this directory/name.
	-- Creates: ExeliusEngine/Exelius/[ProjectFiles]
	location("Exelius")
	characterset("Default")

	-- Premake function that makes this project into a static lib.
	kind("StaticLib")

	-- Set the precompiled headers.
	pchheader("EXEPCH.h")
	pchsource("%{prj.name}/Source/Precompilation/EXEPCH.cpp")

	-- Premake function for getting all of the files to be included into the Project.
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	-- Premake function for allowing <Exelius/...> includes.
	includedirs
	{
		"%{prj.name}/",
		"%{prj.name}/Source/Precompilation/"
	}

	-- Retrieve and set any additional settings for the engine.
	--exeliusDefaults.InitializeEngine(exeliusDepends)
	exeliusDefaults.IncludeLibHeaders(exeliusDepends)

------------------------------------------------------------------------------------------------------
-- Exelius Sandbox Application Creation
------------------------------------------------------------------------------------------------------

printf("---------- Creating Exelius Sandbox Project ---------")

-- Call Premake functions to generate .vcxproj files or platform/compiler specific files.
project("EXESandbox")

	-- Retrieve and Set all of the default settings for the project.
	exeliusDefaults.ProjectDefaults()

	-- Premake function creates the project directory with this directory/name.
	-- Creates: ExeliusEngine/EXESandbox/[ProjectFiles]
	location("EXESandbox")

	-- Premake function that makes this project into a Console Application.
	-- We are choosing to do this for the development process,
	-- and it is likely to change in the future for multiple platforms.
	kind("ConsoleApp")

	-- Premake function for getting all of the files to be included into the Project.
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs("%{prj.name}/Source/")

	-- exeliusDefaults.IncludeLibHeaders(exeliusDepends)
	exeliusDefaults.InitializeEngine(exeliusDepends)
	
	-- Retrieve and Set any additional settings for the project.
	exeliusDefaults.InitializeProject()

------------------------------------------------------------------------------------------------------
-- Additional Creation Steps
------------------------------------------------------------------------------------------------------
printf("----------- Completed Premake Generation  -----------")