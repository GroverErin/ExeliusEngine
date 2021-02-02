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

-- Main Exelius Solution/Workspace and Project Generation Script.

-- The module containing the default settings for the Premake generation.
-- This usually will include the architecture settings as well as the
-- build configurations, and similar settings.
local exeliusDefaults = require("../ExeliusConfigFiles/ExeliusDefaults")

-- The module containing the dependancies and third party tools used by Exelius
local exeliusDepends = require("../Dependencies/ExeliusDependencies")
exeliusDepends.Initialize()

------------------------------------------------------------------------------------------------------
-- Exelius Solution/Workspace Creation
------------------------------------------------------------------------------------------------------

printf("[ExeliusEngine-Premake] Creating ExeliusEngine Solution/Workspace.")

-- Call Premake functions to generate the .sln file or platform/compiler specific files
workspace("ExeliusEngine")
	startproject("EXESandbox")

	-- Retrieve and Set all of the default settings for the workspace.
	exeliusDefaults.WorkspaceDefaults()

------------------------------------------------------------------------------------------------------
-- Exelius Dependancy Creation/Preperation
------------------------------------------------------------------------------------------------------

-- Create all of the Projects for the Third-party tools and other dependancies used by Exelius.
-- Having this allows for the Engine to auto-detect Third-party tools and include them
-- into the Solution/Workspace.
exeliusDepends.GenerateProjects()

------------------------------------------------------------------------------------------------------
-- Exelius Engine Creation
------------------------------------------------------------------------------------------------------

printf("[ExeliusEngine-Premake] Creating Exelius Engine Project.")

-- Call Premake functions to generate .vcxproj files or platform/compiler specific files.
project("Exelius")

	-- Retrieve and Set all of the default settings for the project.
	exeliusDefaults.ProjectDefaults()

	-- Premake function creates the project directory with this directory/name.
	-- Creates: ExeliusEngine/Exelius/[ProjectFiles]
	location("Exelius")
	characterset("Default")

	-- Premake function that makes this project into a static lib.
	-- We are choosing static over dynamic for speed, at the expense of executable size.
	kind("StaticLib")

	-- Set the precompiled headers.
	pchheader("EXEPCH.h")
	pchsource("Exelius/ExeliusCore/Source/Precompilation/EXEPCH.cpp")

	-- Premake function for getting all of the files to be included into the Project.
	files
	{
		"%{prj.name}/ExeliusCore/**.h",
		"%{prj.name}/ExeliusCore/**.cpp"
	}

	-- Premake function for allowing <Exelius/...> includes.
	includedirs
	{
		"%{prj.name}/ExeliusCore/",
		"%{prj.name}/ExeliusCore/Source/Precompilation/"
	}

	-- Retrieve and Set any additional settings for the engine.
	exeliusDefaults.InitializeEngine()

------------------------------------------------------------------------------------------------------
-- Exelius Sandbox Application Creation
------------------------------------------------------------------------------------------------------

printf("[ExeliusEngine-Premake] Creating Exelius Sandbox Project.")

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

	-- Retrieve and Set any additional settings for the project.
	printf("Initializing Sandbox");
	exeliusDefaults.InitializeProject()

------------------------------------------------------------------------------------------------------
-- Additional Creation Steps
------------------------------------------------------------------------------------------------------