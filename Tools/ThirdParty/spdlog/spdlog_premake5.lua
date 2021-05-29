printf("Including spdlog_premake5.lua - This should only appear once.")
------------------------------------------------------------------------------------------------------
--[[
    Exelius Game Engine - Game Development Tool
    Copyright (c) 2020 Exelius Entertainment
    Created By: Erin "Ryukana" Grover

    This Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    License for more details. You should have received a copy of the
    License along with this Engine.
--]]
------------------------------------------------------------------------------------------------------

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

    --------------------------------------------------------------------------------------------------
	-- Windows Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"platforms:x86", "configurations:Debug", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win32/Debug/]],
        }

    filter{"platforms:x86", "configurations:Test", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win32/RelWithDebInfo/]]
        }

    filter{"platforms:x86", "configurations:Release", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win32/MinSizeRel/]]
        }

    filter{"platforms:x64", "configurations:Debug", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win64/Debug/]],
        }

    filter{"platforms:x64", "configurations:Test", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win64/RelWithDebInfo/]]
        }

    filter{"platforms:x64", "configurations:Release", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win64/MinSizeRel/]]
        }
        
    --------------------------------------------------------------------------------------------------
	-- Linux Library Directories
	--------------------------------------------------------------------------------------------------

	filter{"platforms:x86", "configurations:Debug", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin32/Debug/]],
        }

    filter{"platforms:x86", "configurations:Test", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin32/Release/]]
        }

    filter{"platforms:x86", "configurations:Release", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin32/Release/]]
        }

    filter{"platforms:x64", "configurations:Debug", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin64/Debug/]],
        }

    filter{"platforms:x64", "configurations:Test", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin64/Release/]]
        }

    filter{"platforms:x64", "configurations:Release", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin64/Release/]]
        }

    --------------------------------------------------------------------------------------------------
    -- Raspberry Pi Library Directories
    --------------------------------------------------------------------------------------------------

    filter{"platforms:rpi86", "configurations:Debug", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Rpi32/Debug/]],
        }

    filter{"platforms:rpi86", "configurations:Test", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Rpi32/Release/]]
        }

    filter{"platforms:rpi86", "configurations:Release", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Rpi32/Release/]]
        }

    filter{"platforms:rpi64", "configurations:Debug", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Rpi64/Debug/]],
        }

    filter{"platforms:rpi64", "configurations:Test", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Rpi64/Release/]]
        }

    filter{"platforms:rpi64", "configurations:Release", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Rpi64/Release/]]
        }

    filter{}
end

function spdlog.Link(rootdir, exeliusLibDir)

	filter{"configurations:Debug", "system:windows"}
		links
		{
			"spdlogd"
		}

    filter{"configurations:Test", "system:windows"}
		links
		{
			"spdlog"
		}

    filter{"configurations:Release", "system:windows"}
		links
		{
			"spdlog"
		}

    filter{"configurations:Debug", "system:linux"}
		links
		{
			"spdlogd"
		}

    filter{"configurations:Test", "system:linux"}
		links
		{
			"spdlog"
		}

    filter{"configurations:Release", "system:linux"}
		links
		{
			"spdlog"
		}

    filter {}
end

dependencies.Add("spdlog", spdlog)