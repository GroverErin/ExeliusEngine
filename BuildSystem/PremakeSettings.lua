-- Create a new module for premake and get it.
premake.modules.exeliusDefaultSettingsClass = {}
local exeliusDefaultSettings = premake.modules.exeliusDefaultSettingsClass

exeliusDefaultSettings.workspaceName = "ExeliusEngine"
exeliusDefaultSettings.engineProjectName = "Exelius"
exeliusDefaultSettings.exeliusEditorName = "EXESandbox" -- Eventually this will be replaced with the Editor name.
exeliusDefaultSettings.startProjectName = exeliusDefaultSettings.exeliusEditorName

exeliusDefaultSettings.precompiledHeader = "EXEPCH.h"
exeliusDefaultSettings.precompiledHeaderSource = "%{prj.name}/Source/Precompilation/EXEPCH.cpp"

exeliusDefaultSettings.OutputPathConstructor = "%{cfg.buildcfg}_%{cfg.architecture}/%{prj.name}"
exeliusDefaultSettings.BuildOutputDirectory = "bin/" .. exeliusDefaultSettings.OutputPathConstructor
exeliusDefaultSettings.TempOutputDirectory = "obj/" .. exeliusDefaultSettings.OutputPathConstructor

exeliusDefaultSettings.DependencyScriptLocation = path.getabsolute("../tools/thirdparty/")
exeliusDefaultSettings.DependencyBuildOutputDirectory = exeliusDefaultSettings.BuildOutputDirectory

exeliusDefaultSettings.language = "C++"
exeliusDefaultSettings.cppdialect = "C++17"
exeliusDefaultSettings.systemversion = "latest"
exeliusDefaultSettings.warnings = "Extra"
exeliusDefaultSettings.characterset = "Default"
exeliusDefaultSettings.defaultkind = "StaticLib"

function exeliusDefaultSettings.SetGlobalProjectDefaultSettings()
    language(exeliusDefaultSettings.language)
    cppdialect(exeliusDefaultSettings.cppdialect)
    systemversion(exeliusDefaultSettings.systemversion)

    warnings(exeliusDefaultSettings.warnings)

    targetdir(exeliusDefaultSettings.BuildOutputDirectory)
    objdir(exeliusDefaultSettings.TempOutputDirectory)

    characterset(exeliusDefaultSettings.characterset)

    filter {"platforms:x64 or x86_64"}
        architecture("x86_64")

    filter {"platforms:x86 or Win32"}
        architecture("x86")

    filter {"platforms:ARM"}
        architecture("ARM")

    filter {"platforms:ARM64"}
        architecture("ARM64")

    filter {"configurations:Debug"}
        symbols("full")
        runtime("Debug")
        defines("EXE_DEBUG")
        staticruntime("On")

    filter {"configurations:Asan"}
        symbols("full")
        runtime("Debug")
        defines("EXE_DEBUG")
        staticruntime("On")

    filter {"configurations:Test"}
        optimize("On")
        runtime("Release")
        defines("EXE_TEST")
        staticruntime("On")

    filter {"configurations:Release"}
        optimize("On")
        runtime("Release")
        defines("EXE_RELEASE")
        staticruntime("On")

    -- Reset Filters
    filter {}
end

return exeliusDefaultSettings