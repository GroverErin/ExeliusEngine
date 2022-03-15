from buildsystem.PythonConsoleLog import Log as log
from buildsystem.PythonSubmoduleLoader import SubmoduleLoader as submodLoader
from buildsystem.PythonArgHelper import ArgHelper
from buildsystem.PythonPremakeSystem import PremakeSystem as premake
from buildsystem.PythonUtilities import Utilities as utilities
from buildsystem.PythonVulkanSetup import VulkanSetup as vulkan

# Does nothing on non-windows platforms.
log.EnableWindowsColorLog()

# Print Help menu and bail if necessary
if ArgHelper.HandleHelpCommand():
    exit()

isVerbose = ArgHelper.DetectAndSetVerbosity()

# Should force 'git submodule update --init --recursive'?
forceSubmoduleUpdates = ArgHelper.ShouldForceSubmoduleUpdates()

# Validate the version of Python we are using. Version 3 is required.
utilities.Validate()

# Validate or Install VulkanSDK
vulkan.Validate()

# Check if submodules exist, if not, then *always* get.
submodLoader.ValidateSubmodules(forceSubmoduleUpdates)

# Are we done?
if (ArgHelper.OnlyUpdateSubmodules()):
    exit()

premakeInstalled = premake.Validate()

if (premakeInstalled):
    premake.Run(isVerbose)
else:
    log.Error("Premake Required for Project Generation.")

log.Info("Exelius Engine Generation Complete!")
exit()
