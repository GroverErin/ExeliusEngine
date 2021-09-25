local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local imgui = {}

function imgui.GenerateDependencyProject(dependencyRootFolder)
    project("imgui")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        targetdir(exeliusDefaultSettings.BuildOutputDirectory)
        objdir(exeliusDefaultSettings.TempOutputDirectory)

        location(dependencyRootFolder)

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/imconfig.h",
            "%{prj.location}/imgui.h",
            "%{prj.location}/imgui.cpp",
            "%{prj.location}/imgui_draw.cpp",
            "%{prj.location}/imgui_internal.h",
            "%{prj.location}/imgui_widgets.cpp",
            "%{prj.location}/imstb_rectpack.h",
            "%{prj.location}/imstb_textedit.h",
            "%{prj.location}/imstb_truetype.h",
            "%{prj.location}/imgui_demo.cpp" -- May not need this for long.
        }
end

function imgui.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder
    }
end

function imgui.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "imgui"
    }
end

dependencies.AddDependency("imgui", imgui)