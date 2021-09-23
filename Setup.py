import os
import platform
import subprocess
import sys

from BuildSystem.PythonConsoleLog import Log

if sys.argv.__len__() > 1:
    if sys.argv[1] == "-f":
        Log.Log("[Python] Running in Full Mode: Updating Submodules.")

#-------------------------------------------------------------------------------------

Log.Log("[Python] Validating Python Version and Required Packages.")

from BuildSystem.PythonSetup import PythonSetup as Python
# Make sure everything we need for the setup is installed
if not Python.ValidateVersion():
    Log.Error("[Python] Version Validation Failed.")
    exit() # Cannot complete setup.
else:
    Log.Info("[Python] Version Validation Succeeded.")

if not Python.InstallRequiredPackages():
    Log.Error("[Python] Failed to Install Required Packages.")
    exit() # Cannot complete setup.
else:
    Log.Info("[Python] Package Installation Succeeded.")

#-------------------------------------------------------------------------------------

if sys.argv.__len__() > 1:
    if sys.argv[1] == "-f":
        Log.Log("[Python] Updating Submodules")
        subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

#-------------------------------------------------------------------------------------

Log.Log("[Python] Validating Premake Installation.")
from BuildSystem.PremakeSetup import PremakeSetup as Premake

premakeInstalled = Premake.Validate()

#-------------------------------------------------------------------------------------

if (premakeInstalled):
    Log.Log("[Python] Executing Premake.")

    if platform.system() == "Windows":
        subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5.exe"), "vs2019", "--file=./buildsystem/PremakeMain.lua", "nopause"])
    elif platform.system() == "Linux":
        subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5"), "gmake2", "--file=./buildsystem/PremakeMain.lua", "nopause"])
    else:
        Log.Error(f"[Python] OS '{platform.system()}' Not Supported.")
        exit() # Cannot complete setup.

    Log.Info("[Python] Setup completed!")
else:
    Log.Error("[Python] Premake Required for Project Generation.")
