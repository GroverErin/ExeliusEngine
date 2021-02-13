printf("Including spdlog_premake5.lua - This should only appear once.")
local dependencies = require("Tools/ThirdParty/DependencyManager")

local spdlog = 
{
    shortname = "spdlog",
    description = "Outputs arguments to console."
}

function spdlog.Include(rootDir)
    includedirs
    {
        rootDir .. [[include]],
    }
end

dependencies.Add("spdlog", spdlog)