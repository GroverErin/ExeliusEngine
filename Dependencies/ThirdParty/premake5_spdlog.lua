local dependencies = require("../Dependencies/ExeliusDependencies")

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