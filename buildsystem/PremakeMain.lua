local log = require("PremakeConsoleLog")
local engineGenerator = require("PremakeEngineGenerator")
local dependencyGenerator = require("PremakeDependancyGenerator")

-- Define new available arguments for this script.
newoption
{
    trigger =           "configuration",
    value =             "config",
    description =       "The configuration that Premake should generate. Ignored on Windows.",
    allowed = {
        { "Release",    "Release (Lightweight, Fast - Optimized w/o Debug)" },
        { "Test",       "Test (Heavy, Fast - Optimized w/ Debug)" },
        { "ASan",       "Same as Debug, with Address Sanitizer. (EXTREMELY Slow and Heavy)" },
        { "Debug",      "Debug (Heavy, Slow - Unoptimized)" }
    },
    default =           "Release"
}

newoption
{
    trigger =           "architecture",
    value =             "arch",
    description =       "The architecture that Premake should generate for (Windows) or binary to be built (Unix).",
    allowed = {
        { "ARM",        "ARM 32 Bit" },
        { "ARM64",      "ARM 64 Bit" },
        { "x86",        "32 Bit" },
        { "x86_64",     "64 Bit" },
        { "Win32",      "Same as x86" },
        { "x64",        "Same as x86_64" },
    },
    default =           "x86"
}

log.Log("[Premake] Beginning Premake File Generation.")

------------------------------------------------------------------------------------------------------
-- Exelius Solution/Workspace Creation
------------------------------------------------------------------------------------------------------

log.Log("[Premake] Creating ExeliusEngine Solution/Workspace.")
engineGenerator.GenerateEngineWorkspace()
log.Info("[Premake] ExeliusEngine Solution/Workspace Created.")

log.Log("[Premake] Creating ExeliusEngine Dependencies.")
if dependencyGenerator.GenerateDependencies() then
    log.Info("[Premake] ExeliusEngine Dependencies Created.")
else
    return -1
end

log.Log("[Premake] Creating ExeliusEngine Project.")
engineGenerator.GenerateEngineProject()
dependencyGenerator.IncludeDependencies()
log.Info("[Premake] ExeliusEngine Project Created.")

log.Log("[Premake] Creating ExeliusEditor Project.")
engineGenerator.GenerateEditorProject()
dependencyGenerator.IncludeDependencies()
engineGenerator.LinkEngineToProject()
dependencyGenerator.LinkDependencies()
log.Info("[Premake] ExeliusEditor Project Created.")

log.Info("[Premake] Engine Generation Complete!")