-- Create a new module for premake and get it.
premake.modules.exeliusDefaultSettingsClass = {}
local exeliusDefaultSettings = premake.modules.exeliusDefaultSettingsClass

exeliusDefaultSettings.workspaceName = "exeliusengine"
exeliusDefaultSettings.engineProjectName = "exelius"
exeliusDefaultSettings.exeliusEditorName = "exeliuseditor"
exeliusDefaultSettings.startProjectName = exeliusDefaultSettings.exeliusEditorName

exeliusDefaultSettings.precompiledHeader = "EXEPCH.h"
exeliusDefaultSettings.precompiledHeaderSource = "%{wks.location}/%{prj.name}/source/precompilation/EXEPCH.cpp"

exeliusDefaultSettings.OutputPathConstructor = "%{cfg.buildcfg}_%{cfg.architecture}/%{prj.name}"
exeliusDefaultSettings.BuildOutputDirectory = "%{wks.location}/bin/" .. exeliusDefaultSettings.OutputPathConstructor
exeliusDefaultSettings.TempOutputDirectory = "%{wks.location}/obj/" .. exeliusDefaultSettings.OutputPathConstructor

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
        staticruntime("off")

    filter {"configurations:Asan"}
        symbols("full")
        runtime("Debug")
        defines("EXE_DEBUG")
        staticruntime("off")

    filter {"configurations:Test"}
        optimize("On")
        runtime("Release")
        defines("EXE_TEST")
        staticruntime("off")

    filter {"configurations:Release"}
        optimize("On")
        runtime("Release")
        defines("EXE_RELEASE")
        staticruntime("off")

    filter {"system:windows"}
        defines("EXE_WINDOWS")

    filter {"system:linux"}
        defines("EXE_LINUX")

    -- Reset Filters
    filter {}
end

return exeliusDefaultSettings