local log = require("PremakeConsoleLog")
local defaultSettings = require("PremakeSettings")

-- Create a new module for premake and get it.
premake.modules.exeliusGeneratorClass = {}
local exeliusGenerator = premake.modules.exeliusGeneratorClass

function GenerateWindowsWorkspace()
    filter {"system:windows"}
        -- Generate the different build configurations for Visual Studio.
        configurations
        {
            "Debug",    -- Contains no compiler optimized code and no packaged assets. (Slow and Heavy)
            "ASan",     -- Same as Debug, with Address Sanitizer.(EXTREMELY Slow and Heavy)
            "Test",     -- Contains compiler optimized code and no packaged assets. (Fast and Heavy)
            "Release"   -- Contains compiler optimized code and packaged assets. (Fast and Light)
        }

        platforms
        {
            "x64",
            "x86"
        }

        -- Set the compilation process to be multithreaded, this only effects MSVC... not sure how to speed up Linux.
        flags
        {
            "MultiProcessorCompile"
        }

        -- Reset filters
    filter {}
end

function GenerateLinuxWorkspace()
    filter {"system:linux"}
        -- Set the configuration based on the command line argument supplied from Setup.py.
        configurations
        {
            _OPTIONS["configuration"]
        }

        -- Set the architecture based on the command line argument supplied from Setup.py.
        platforms
        {
            _OPTIONS["architecture"]
        }

    -- Reset filters
    filter {}
end

function SetWindowsPostBuildCommands()
    filter {"system:windows"}
        postbuildcommands
        {
            [[copy "%{wks.location}tools\templates\engine_config.ini" "%{cfg.targetdir}\engine_config.ini" /y /a]], -- Copy the ini into the final build directory.
            [[copy "%{wks.location}tools\templates\engine_config.ini" "engine_config.ini" /y /a]], -- Copy the ini into the debuggers directory.
            [[xcopy "assets" "%{cfg.targetdir}\assets\" /y /q /e]] -- Copy assets into final build directory.
        }
end

function SetLinuxPostBuildCommands()
    -- TODO: This will need to be tested and worked on.
    filter {"system:linux"}
        postbuildcommands
        {
            [[cp -f tools/templates/engine_config.ini %{cfg.targetdir}/engine_config.ini]]
        }
end

function exeliusGenerator.GenerateEngineWorkspace()
    workspace(defaultSettings.workspaceName)
        startproject(defaultSettings.startProjectName)

        local workspacePath = os.realpath("../")
        location(workspacePath)

        GenerateWindowsWorkspace()
        GenerateLinuxWorkspace()
end

function exeliusGenerator.GenerateEngineProject()
    project(defaultSettings.engineProjectName)
        defaultSettings.SetGlobalProjectDefaultSettings()
        local enginePath = os.realpath("%{wks.location}/" .. defaultSettings.engineProjectName)

        -- Use a relative path here only because it logs nicer. Totally unnessesary.
        local pathToLog = os.realpath("../" .. defaultSettings.engineProjectName)
        log.Log("[Premake] Generating Engine at Path: " .. pathToLog)

        location(enginePath)
        kind(defaultSettings.defaultkind)

        pchheader("EXEPCH.h")
        pchsource("../%{prj.name}/source/precompilation/EXEPCH.cpp")
        --pchsource("%{prj.location}/Source/Precompilation/EXEPCH.cpp") -- Doesn't work. %{prj.location} or %{wks.location} will not correctly expand when used in pchsource.

        files
        {
            "%{prj.location}/**.h",
            "%{prj.location}/**.cpp"
        }

        includedirs
        {
            "%{prj.location}/",
            "%{prj.location}/source/precompilation/"
        }
end

function exeliusGenerator.GenerateEditorProject()
    project(defaultSettings.exeliusEditorName)
        defaultSettings.SetGlobalProjectDefaultSettings()

        local editorPath = os.realpath("%{wks.location}/" .. defaultSettings.exeliusEditorName)

        -- Use a relative path here only because it logs nicer. Totally unnessesary.
        local pathToLog = os.realpath("../" .. defaultSettings.exeliusEditorName)
        log.Log("[Premake] Generating Editor at Path: " .. pathToLog)

        location(editorPath)
        kind("ConsoleApp")

        files
        {
            "%{wks.location}/%{prj.name}/source/**.h",
            "%{wks.location}/%{prj.name}/source/**.cpp"
        }

        includedirs
        {
            "%{wks.location}/%{prj.name}/source/"
        }
end

function exeliusGenerator.LinkEngineToProject()
    local engineIncludePath = os.realpath("%{wks.location}/" .. defaultSettings.engineProjectName)

    -- Use a relative path here only because it logs nicer. Totally unnessesary.
    local pathToLog = os.realpath("../" .. defaultSettings.engineProjectName)
    log.Log("[Premake] Linking Engine From Path: " .. pathToLog)

    links
    {
        defaultSettings.engineProjectName
    }

    includedirs
    {
        engineIncludePath
    }

    SetWindowsPostBuildCommands()
    SetLinuxPostBuildCommands()
end

return exeliusGenerator