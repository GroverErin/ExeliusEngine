import os
import shutil
from pathlib import Path

from buildsystem.PythonConsoleLog import Log as log
from buildsystem.PythonUtilities import Utilities as utilities
import buildsystem.PythonConfig as const

from urllib.request import urlopen

class VulkanSetup:
    @classmethod
    def Validate(self):
        if (not self.__CheckVulkanSDK()):
            log.Error("Vulkan SDK not installed correctly.")
            return

        if (not self.__CopyVulkanSDKReleaseLibs()):
            log.Error("Unable to Copy Vulkan Release Libs.")
            return
            
        if (not self.__CheckVulkanSDKDebugLibs()):
            log.Error("Vulkan SDK debug libs not found.")

    @classmethod
    def __CheckVulkanSDK(self):
        vulkanSDK = os.environ.get(const.VULKAN_ENVIRONMENT_VARIABLE)
        if (vulkanSDK is None):
            log.Warn("You don't have the Vulkan SDK installed!")
            self.__InstallVulkanSDK()
            return False
        else:
            log.Info(f"Located Vulkan SDK at {vulkanSDK}")

        if (const.VULKAN_REQUIRED_VERSION not in vulkanSDK):
            log.Warn(f"You don't have the correct Vulkan SDK version! (Engine requires {const.VULKAN_REQUIRED_VERSION})")
            self.__InstallVulkanSDK()
            return False
    
        log.Info(f"Correct Vulkan SDK located at {vulkanSDK}")
        return True

    @classmethod
    def __InstallVulkanSDK(self):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(const.VULKAN_REQUIRED_VERSION))).lower().strip()[:1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        vulkanInstallURL = const.VULKAN_INSTALL_URL
        vulkanPath = const.VULKAN_INSTALL_LOCATION
        log.Info("Downloading {0:s} to {1:s}".format(vulkanInstallURL, vulkanPath))
        utilities.DownloadFile(vulkanInstallURL, vulkanPath)
        log.Info("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(vulkanPath))
        log.Warn("Re-run this script after installation!")
        quit()

    @classmethod
    def __CheckVulkanSDKDebugLibs(self):
        shadercdLib = Path(const.VULKAN_SHADERC_DEBUG_LIB)
        
        VulkanSDKDebugLibsURLlist = [
            const.VULKAN_WINDOWS_DEBUG_LIB_URL_A,
            const.VULKAN_WINDOWS_DEBUG_LIB_URL_B
        ]
        
        if not shadercdLib.exists():
            log.Warn(f"No Vulkan SDK debug libs found. (Checked {shadercdLib})")
            vulkanPath = const.VULKAN_SHADERC_DEBUG_LIB_INSTALL
            utilities.DownloadFile(VulkanSDKDebugLibsURLlist, vulkanPath)
            log.Info(f"Extracting to '{vulkanPath}'")
            utilities.UnpackFile(vulkanPath, const.VULKAN_SDK_PATH)
            log.Info(f"Vulkan SDK debug libs installed at {os.path.abspath(const.VULKAN_SDK_PATH)}")
        else:
            log.Info(f"Vulkan SDK debug libs located at {os.path.abspath(const.VULKAN_SDK_PATH)}")
        return True

    @classmethod
    def __CopyVulkanSDKReleaseLibs(self):
        vulkanSDK = os.environ.get(const.VULKAN_ENVIRONMENT_VARIABLE)
        vulkanSDKX86Path = vulkanSDK + "/Bin32/"
        vulkanSDKX64Path = vulkanSDK + "/Bin/"

        os.makedirs(os.path.dirname(const.VULKAN_RELEASE_DLL_INSTALL_PATH + "Bin32/"), exist_ok=True)
        os.makedirs(os.path.dirname(const.VULKAN_RELEASE_DLL_INSTALL_PATH + "Bin/"), exist_ok=True)

        for libName in const.VULKAN_RELEASE_DLLS:
            shutil.copy(vulkanSDKX86Path + libName, const.VULKAN_RELEASE_DLL_INSTALL_PATH + "Bin32/")
            shutil.copy(vulkanSDKX64Path + libName, const.VULKAN_RELEASE_DLL_INSTALL_PATH + "Bin/")
        return True
