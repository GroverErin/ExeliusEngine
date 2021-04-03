printf("Including libsodium_premake5.lua - This should only appear once.")
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

local libsodium = 
{
	location = "libsodium"
}

function libsodium.Include(rootDir)
	includedirs
	{
		rootDir .. [[include/]]
	}

    --------------------------------------------------------------------------------------------------
	-- Windows Library Directories
	--------------------------------------------------------------------------------------------------

    filter{"platforms:Win32", "configurations:Debug", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win32/Debug/]],
        }

    filter{"platforms:Win32", "configurations:Test", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win32/Release/]]
        }

    filter{"platforms:Win32", "configurations:Release", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win32/Release/]]
        }

    filter{"platforms:x64", "configurations:Debug", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win64/Debug/]],
        }

    filter{"platforms:x64", "configurations:Test", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win64/Release/]]
        }

    filter{"platforms:x64", "configurations:Release", "system:windows"}
        libdirs
        {
            rootDir .. [[lib/Win64/Release/]]
        }
    
    --------------------------------------------------------------------------------------------------
    -- Linux Library Directories
    --------------------------------------------------------------------------------------------------

    filter{"platforms:Win32", "configurations:Debug", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin32/Debug/]],
        }

    filter{"platforms:Win32", "configurations:Test", "system:linux"}
        libdirs
        {
            rootDir .. [[lib/Lin32/Release/]]
        }

    filter{"platforms:Win32", "configurations:Release", "system:linux"}
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

    filter{}
end

function libsodium.Link(rootdir, exeliusLibDir)

	filter{"system:windows"}
		links
		{
			"libsodium"
		}

	filter{"system:linux"}
		links
		{
			"libsodium"
		}

	filter {}
end

dependencies.Add("libsodium", libsodium)