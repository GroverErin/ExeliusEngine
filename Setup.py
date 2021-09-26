import os
import platform
import subprocess
import sys

from buildsystem.PythonConsoleLog import Log as log

# Check if we have the submodules.
    # If not, then always get them.
    # Else, Check if we want to update the submodules.
            # If we want to ONLY update submodules, then do so and bail.
            # Else, Update the submodules and continue the operation.











if sys.argv.__len__() > 1:
    if sys.argv[1] == "-f":
        log.Log("[Python] Running in Full Mode: Updating Submodules.")

#-------------------------------------------------------------------------------------

log.Log("[Python] Validating Python Version and Required Packages.")

from buildsystem.PythonSetup import PythonSetup as python
# Make sure everything we need for the setup is installed
if not python.ValidateVersion():
    log.Error("[Python] Version Validation Failed.")
    exit() # Cannot complete setup.
else:
    log.Info("[Python] Version Validation Succeeded.")

if not python.InstallRequiredPackages():
    log.Error("[Python] Failed to Install Required Packages.")
    exit() # Cannot complete setup.
else:
    log.Info("[Python] Package Installation Succeeded.")

#-------------------------------------------------------------------------------------

if sys.argv.__len__() > 1:
    if sys.argv[1] == "-f":
        log.Log("[Python] Updating Submodules")
        subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

#-------------------------------------------------------------------------------------

log.Log("[Python] Validating Premake Installation.")
from buildsystem.PremakeSetup import PremakeSetup as premake

premakeInstalled = premake.Validate()

#-------------------------------------------------------------------------------------

if (premakeInstalled):
    log.Log("[Python] Executing Premake.")

    if platform.system() == "Windows":
        subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5.exe"), "vs2019", "--file=./buildsystem/PremakeMain.lua", "nopause"])
    elif platform.system() == "Linux":
        subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5"), "gmake2", "--file=./buildsystem/PremakeMain.lua", "nopause"])
    else:
        log.Error(f"[Python] OS '{platform.system()}' Not Supported.")
        exit() # Cannot complete setup.

    log.Info("[Python] Setup completed!")
else:
    log.Error("[Python] Premake Required for Project Generation.")
