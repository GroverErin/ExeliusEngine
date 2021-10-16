import os
import platform
import subprocess

from buildsystem.PythonUtilities import Utilities as utilities
from buildsystem.PythonConsoleLog import Log as log
import buildsystem.PythonConfig as const

class PremakeSystem:
    @classmethod
    def Validate(self):
        log.Info("Validating Premake Installation.")
        if (not self._ValidatePremake()):
            log.Error("Premake Not Installed.")
            return False

        log.Info("Premake Validation Complete.")
        return True

    @classmethod
    def Run(self, isVerbose):
        log.Info("Executing Premake.")

        verbositySetting = "--verbosity=low"
        if isVerbose:
            verbositySetting = "--verbosity=high"

        if 'armv' in platform.machine():
            subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5"), "gmake", "--file=./buildsystem/PremakeMain.lua", "--architecture=ARM64", "--configuration=Release", verbositySetting, "nopause"])
        elif platform.system() == "Windows":
            subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5.exe"), "vs2019", "--file=./buildsystem/PremakeMain.lua", verbositySetting, "nopause"])
        elif platform.system() == "Linux":
            subprocess.call([os.path.abspath("./tools/thirdparty/premake/premake5"), "gmake2", "--file=./buildsystem/PremakeMain.lua", verbositySetting, "nopause"])
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))
            exit() # Cannot complete setup.

        log.Info("Premake Generation Completed.")

    @classmethod
    def _ValidatePremake(self):
        if platform.system() == "Windows":
            premakeExecutablePath = os.path.abspath(const.PREMAKE_UNPACK_PATH + 'premake5.exe')
        elif platform.system() == "Linux":
            premakeExecutablePath = os.path.abspath(const.PREMAKE_UNPACK_PATH + 'premake5')
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))

        if (not os.path.isfile(premakeExecutablePath)):
            log.Warn("Premake Not Found at Location: {0:s}".format(os.path.abspath(const.PREMAKE_UNPACK_PATH)))
            return self._UnpackPremake()

        log.Info("Premake Validation Succeeded.")
        return True

    @classmethod
    def _UnpackPremake(self):
        log.Info("Unpacking Premake.")
        packedPremakeFilename = ""
        if 'armv' in platform.machine():
            packedPremakeFilename = const.PREMAKE_RPI_TAR_FILE
        elif platform.system() == "Windows":
            packedPremakeFilename = const.PREMAKE_WINDOWS_ZIP_FILE
        elif platform.system() == "Linux":
            packedPremakeFilename = const.PREMAKE_LINUX_TAR_FILE
        else:
            log.Error("OS '{0:s}' Not Supported.".format(platform.system()))
            return False

        premakePackedPath = os.path.abspath(const.PREMAKE_PACKED_PATH + packedPremakeFilename)
        premakeUnpackPath = os.path.abspath(const.PREMAKE_UNPACK_PATH)

        return utilities.UnpackFile(premakePackedPath, premakeUnpackPath)