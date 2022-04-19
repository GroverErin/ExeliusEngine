local dependencies = require("PremakeDependancyGenerator")
local exeliusDefaultSettings = require("PremakeSettings")

local vulkan = {}

function vulkan.GenerateDependencyProject(dependencyRootFolder)
end

function vulkan.IncludeDependency(dependencyRootFolder)
    local vulkanSDKPath = os.getenv("VULKAN_SDK")

    includedirs
    {
        vulkanSDKPath .. "/Include/"
    }

    filter {"architecture:x86", "configurations:Release"}
        libdirs
        {
            vulkanSDKPath .. "/Lib32/"
        }
    filter {"architecture:x86", "configurations:Debug"}
        libdirs
        {
            dependencyRootFolder .. "Debug/Lib32/"
        }

    filter {"architecture:x86_64", "configurations:Release"}
        libdirs
        {
            vulkanSDKPath .. "/Lib/"
        }
    filter {"architecture:x86_64", "configurations:Debug"}
        libdirs
        {
            dependencyRootFolder .. "Debug/Lib/"
        }
    filter {}
end

function vulkan.LinkDependency(dependencyRootFolder)

    filter {"configurations:Debug"}
        links
        {
            "shaderc_sharedd",
            "spirv-cross-cored",
            "spirv-cross-glsld"
        }

    filter {"configurations:Release"}
        links
        {
            "shaderc_shared",
            "spirv-cross-core",
            "spirv-cross-glsl"
        }

    filter {}
end

dependencies.AddDependency("vulkan", vulkan)