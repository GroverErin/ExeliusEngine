local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local imguinodeeditor = {}

function imguinodeeditor.GenerateDependencyProject(dependencyRootFolder)
    project("imguinodeeditor")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        targetdir(exeliusDefaultSettings.BuildOutputDirectory)
        objdir(exeliusDefaultSettings.TempOutputDirectory)

        warnings("Off")
        location(dependencyRootFolder)

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/crude_json.cpp",
            "%{prj.location}/crude_json.h",
            "%{prj.location}/imgui_bezier_math.h",
            "%{prj.location}/imgui_bezier_math.inl",
            "%{prj.location}/imgui_canvas.cpp",
            "%{prj.location}/imgui_canvas.h",
            "%{prj.location}/imgui_extra_math.h",
            "%{prj.location}/imgui_extra_math.inl",
            "%{prj.location}/imgui_node_editor.cpp",
            "%{prj.location}/imgui_node_editor.h",
            "%{prj.location}/imgui_node_editor_api.cpp",
            "%{prj.location}/imgui_node_editor_internal.h",
            "%{prj.location}/imgui_node_editor_internal.inl",
        }

        includedirs
        {
            "%{prj.location}/../imgui/"
        }
end

function imguinodeeditor.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "/misc/imgui_node_editor.natvis",
            dependencyRootFolder .. "/misc/crude_json.natvis"
        }
    filter {}
end

function imguinodeeditor.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "imguinodeeditor"
    }
end

dependencies.AddDependency("imguinodeeditor", imguinodeeditor)