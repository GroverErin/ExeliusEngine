local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local imguizmo = {}

function imguizmo.GenerateDependencyProject(dependencyRootFolder)
    project("imguizmo")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        targetdir(exeliusDefaultSettings.BuildOutputDirectory)
        objdir(exeliusDefaultSettings.TempOutputDirectory)

        location(dependencyRootFolder)

        warnings("Off")

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/GraphEditor.cpp",
            "%{prj.location}/GraphEditor.h",
            "%{prj.location}/ImCurveEdit.cpp",
            "%{prj.location}/ImCurveEdit.h",
            "%{prj.location}/ImGradient.cpp",
            "%{prj.location}/ImGradient.h",
            "%{prj.location}/ImGuizmo.cpp",
            "%{prj.location}/ImGuizmo.h",
            "%{prj.location}/ImSequencer.cpp",
            "%{prj.location}/ImSequencer.h",
            "%{prj.location}/ImZoomSlider.h"
        }

        includedirs
        {
            "%{prj.location}/../imgui/"
        }
end

function imguizmo.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder
    }
end

function imguizmo.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "imguizmo"
    }
end

dependencies.AddDependency("imguizmo", imguizmo)