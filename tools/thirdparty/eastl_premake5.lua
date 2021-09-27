local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local eastl = {}

function eastl.GenerateDependencyProject(dependencyRootFolder)
    project("eastl")
        kind("StaticLib")

        exeliusDefaultSettings.SetGlobalProjectDefaultSettings()

        location(dependencyRootFolder)

        filter {"system:linux"}
            pic("On")

        filter {}

        files
        {
            "%{prj.location}/source/**.cpp"
        }

        includedirs
        {
            "%{prj.location}/include/",
            "%{prj.location}/test/packages/EABase/include/Common/"
        }

        defines
        {
            "EASTL_EASTDC_VSNPRINTF=0",
            "_SSIZE_T_",
            "_SSIZE_T_DEFINED",
            "_CRT_SECURE_NO_WARNINGS"
        }
end

function eastl.IncludeDependency(dependencyRootFolder)
    includedirs
    {
        dependencyRootFolder .. "include/",
        dependencyRootFolder .. "test/packages/EABase/include/Common/"
    }

    filter {"system:windows"}
        files
        {
            dependencyRootFolder .. "doc/EASTL.natvis"
        }
    filter {}

    defines
    {
        "EASTL_EASTDC_VSNPRINTF=0",
        "_SSIZE_T_",
        "_SSIZE_T_DEFINED",
        "_CRT_SECURE_NO_WARNINGS"
    }
end

function eastl.LinkDependency(dependencyRootFolder, exeliusLibDir)
    links
    {
        "eastl"
    }

    filter {"system:linux"}
        links
        {
            "stdc++fs"
        }
    filter {}
end

dependencies.AddDependency("eastl", eastl)